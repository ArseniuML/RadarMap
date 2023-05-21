#include "generatetilestool.h"

#include <QFileDialog>
#include <QDir>

#include <utility>

#include <half.h>
#include <ImfOutputFile.h>
#include <ImfChannelList.h>
#include <IexErrnoExc.h>

#include <RLib/Collection/Array2D.h>
#include <RLib/Serialization/EXR.h>

Q_DECLARE_METATYPE(GenerateTilesParams);

TilesGenerator::TilesGenerator()
{
	connect(this, SIGNAL(hideProgress()), &mProgress, SLOT(hide()), Qt::QueuedConnection);
	connect(this, SIGNAL(progress(int)), &mProgress, SLOT(setProgress(int)), Qt::QueuedConnection);
	connect(&mProgress, SIGNAL(terminated()), this, SLOT(stopGenerating()), Qt::DirectConnection);
}

void TilesGenerator::showProgress()
{
	mProgress.setProgress(0);
	mProgress.show();
}

const char* kernelSource = 
"__kernel void makeTile\n"
"(\n"
		"__read_only image2d_t srcMap,\n"
		"__read_only image2d_t srcDEM,\n"
		"__global half *dstMap\n"
")\n"
"{\n"
	"const sampler_t samp =  CLK_NORMALIZED_COORDS_FALSE |\n" 	
										"CLK_ADDRESS_CLAMP_TO_EDGE |\n"	
										"CLK_FILTER_NEAREST;\n"
	"int2 coord;\n"
	"coord.x = get_global_id(0);\n"
	"coord.y = get_global_id(1);\n"
	
	"float m = read_imagef(srcMap, samp, coord).x;\n"
	
	"float h1 = read_imagef(srcDEM, samp, (int2)(coord.x, coord.y + 2)).x;\n"
	"float h2 = read_imagef(srcDEM, samp, (int2)(coord.x + 1, coord.y + 2)).x;\n"
	"float h3 = read_imagef(srcDEM, samp, (int2)(coord.x + 2, coord.y + 2)).x;\n"
	"float h4 = read_imagef(srcDEM, samp, (int2)(coord.x, coord.y + 1)).x;\n"
	"float h5 = read_imagef(srcDEM, samp, (int2)(coord.x + 2, coord.y + 1)).x;\n"
	"float h6 = read_imagef(srcDEM, samp, (int2)(coord.x, coord.y)).x;\n"
	"float h7 = read_imagef(srcDEM, samp, (int2)(coord.x + 1, coord.y)).x;\n"
	"float h8 = read_imagef(srcDEM, samp, (int2)(coord.x + 2, coord.y)).x;\n"

	"float bx = h1 - h3 + 2*(h4 - h5) + h6 - h8;\n"
	"float by = h6 - h1 + 2*(h7 - h2) + h8 - h3;\n"

	"int offset = (coord.y *  get_global_size(0) + coord.x) * 3;\n"
	"vstore_half_rtn(m, offset, dstMap);\n"
	"vstore_half_rtn(bx, offset + 1, dstMap);\n"
	"vstore_half_rtn(by, offset + 2, dstMap);\n"
"}\n";
	
void TilesGenerator::init()
{
	try 
	{
		cl::vector<cl::Platform> platformList;
		cl::Platform::get(&platformList);
		cl_context_properties cprops[3] = 
		{
			CL_CONTEXT_PLATFORM, 
			(cl_context_properties)(platformList[0])(), 
			0
		};

		mpclContext = new cl::Context(CL_DEVICE_TYPE_GPU, cprops);
		cl::vector<cl::Device> devices = mpclContext->getInfo<CL_CONTEXT_DEVICES>();
		mpclCommandQueue = new cl::CommandQueue(*mpclContext, devices[0], 0);

		cl::Program::Sources source(1, std::make_pair(kernelSource, strlen(kernelSource)+1));
		cl::Program program(*mpclContext, source);
		program.build(devices);
		mpclKernel = new cl::Kernel(program, "makeTile");
	} 
	catch (cl::Error& error) 
	{
		int qq = 0;
	}
}

void TilesGenerator::destroy()
{
	delete mpclKernel;
	delete mpclCommandQueue;
	delete mpclContext;
}

void TilesGenerator::generate( QVariant params )
{
	GenerateTilesParams par = params.value<GenerateTilesParams>();
	mStopGenerating = false;

	try
	{
		GDALRasterBandH hrbMap = GDALGetRasterBand(par.DsMap, 1);
		GDALRasterBandH hrbDEM = GDALGetRasterBand(par.DsDEM, 1); 

		RLib::Collection::Array2D<float, 1> arMap(par.TileSize.width() + 2, par.TileSize.height() + 2);
		cl::Image2D imMap
		(
			*mpclContext, CL_MEM_READ_ONLY, cl::ImageFormat(CL_R, CL_FLOAT), 
			arMap.Width(), arMap.Height()
		);
	
		RLib::Collection::Array2D<float, 1> arDEM(par.TileSize.width() + 4, par.TileSize.height() + 4);
		cl::Image2D imDEM
		(
			*mpclContext, CL_MEM_READ_ONLY, cl::ImageFormat(CL_R, CL_FLOAT), 
			arDEM.Width(), arDEM.Height()
		);

		RLib::Collection::Array2D<half, 3> arTile(par.TileSize.width() + 2, par.TileSize.height() + 2);
		//RLib::Collection::Array2D<float, 3> arTilef(par.TileSize.width() + 2, par.TileSize.height() + 2);
		
		cl::Buffer bufTile
		(
			*mpclContext, CL_MEM_READ_WRITE, 
			arTile.ByteSize()
		);

		int tileNum = 0;
		int numTiles = par.GridSize.width() * par.GridSize.height();

		for (int tileY = 0; tileY < par.GridSize.height() && !mStopGenerating; ++tileY)
		{
			QString rowDir = par.TargetPath + QString().sprintf("\\r%03d", par.GridSize.height() - tileY - 1 + par.GridOffset.height());
			QDir().mkpath(rowDir);
			
			for (int tileX = 0; tileX < par.GridSize.width() && !mStopGenerating; ++tileX)
			{
				int x0 = tileX * par.TileSize.width();
				int y0 = tileY * par.TileSize.height();

				cl::size_t<3> origin, region;

				GDALRasterIO
				(
					hrbMap,	GF_Read,
					x0 + par.PaddingMap + 1, 
					y0 + par.PaddingMap + 1,
					arMap.Width(), 
					arMap.Height(),
					&arMap(0, 0)[0],
					arMap.Width(), 
					arMap.Height(),
					GDT_Float32,
					0,	0
				);

				origin[0] = 0;
				origin[1] = 0;
				origin[2] = 0;

				region[0] = arMap.Width();
				region[1] = arMap.Height();
				region[2] = 1;

				mpclCommandQueue->enqueueWriteImage(imMap, CL_TRUE, origin, region, 0, 0, &arMap(0, 0)[0]);

				GDALRasterIO
				(
					hrbDEM,	GF_Read,
					x0 + par.PaddingDEM, 
					y0 + par.PaddingDEM,
					arDEM.Width(), 
					arDEM.Height(),
					&arDEM(0, 0)[0],
					arDEM.Width(), 
					arDEM.Height(),
					GDT_Float32,
					0,	0
				);

				origin[0] = 0;
				origin[1] = 0;
				origin[2] = 0;

				region[0] = arDEM.Width();
				region[1] = arDEM.Height();
				region[2] = 1;
			
				mpclCommandQueue->enqueueWriteImage(imDEM, CL_TRUE, origin, region, 0, 0, &arDEM(0, 0)[0]);

				mpclKernel->setArg(0, imMap);
				mpclKernel->setArg(1, imDEM);
				mpclKernel->setArg(2, bufTile);

				mpclCommandQueue->enqueueNDRangeKernel
				(
					*mpclKernel, 
					cl::NullRange, cl::NDRange(arTile.Width(), arTile.Height()),
					cl::NDRange(1, 1), NULL, NULL
				);
				mpclCommandQueue->enqueueReadBuffer(bufTile, CL_TRUE, 0, arTile.ByteSize(), &arTile(0, 0)[0]);

				/*for (int i = 0; i < arTilef.Size(); ++i)
				{
					arTilef(i)[0] = arTile(i)[0];
					arTilef(i)[1] = arTile(i)[1];
					arTilef(i)[2] = arTile(i)[2];
				}*/

				QString blockFileName = par.TargetPath + QString().sprintf("\\r%03d\\c%03d", par.GridSize.height() - tileY  - 1 + par.GridOffset.height(), tileX + par.GridOffset.width());
				blockFileName = blockFileName + par.Suffix + QString(".exr");
				RLib::Serialization::EXR::SaveEXR<Imf::HALF>(arTile, blockFileName.toStdString().c_str());

				++tileNum;
				emit progress(100.0 * float(tileNum) / float(numTiles));
			}
		}
	}
	catch (cl::Error& er)
	{
		int qq = 0;
	}

	emit hideProgress();
}


GenerateTilesTool::GenerateTilesTool(QWidget *parent): 
QDialog(parent)
{
	ui.setupUi(this);
	mTilesGenerator.moveToThread(&mThread);
	
	connect(&mThread, SIGNAL(started()), &mTilesGenerator, SLOT(init()), Qt::QueuedConnection);
	connect(this, SIGNAL(destroyGenerator()), &mTilesGenerator, SLOT(destroy()), Qt::BlockingQueuedConnection);
	connect(this, SIGNAL(generate(QVariant)), &mTilesGenerator, SLOT(generate(QVariant)), Qt::QueuedConnection);
	
	mThread.start();
}

GenerateTilesTool::~GenerateTilesTool()
{
	mTilesGenerator.stopGenerating();
	emit destroyGenerator();
	
	mThread.quit();
	mThread.wait();
	
	if (mParams.DsMap) GDALClose(mParams.DsMap);
	if (mParams.DsDEM) GDALClose(mParams.DsDEM);
}

void GenerateTilesTool::openMapDataset()
{
	QString fileName = 	QFileDialog::getOpenFileName(this, tr("Open map dataset"), 
								ui.mapDatasetLineEdit->text(), tr("Datasets (*.tif; *.tiff)"));
	if (!fileName.isNull())
	{
		if (mParams.DsMap) GDALClose(mParams.DsMap);
		mParams.DsMap = GDALOpen(fileName.toStdString().c_str(), GA_ReadOnly);
		if (mParams.DsMap)
		{
			GDALRasterBandH hrb = GDALGetRasterBand(mParams.DsMap, 1);
			if (hrb)
			{
				int width = GDALGetRasterBandXSize(hrb);
				int height = GDALGetRasterBandYSize(hrb);

				ui.mapWidthLineEdit->setText(QString::number(width));
				ui.mapHeightLineEdit->setText(QString::number(height));

				ui.mapDatasetLineEdit->setText(fileName);
				refresh();
			}
		}
	}
}

void GenerateTilesTool::openDEM_Dataset()
{
	QString fileName = 	QFileDialog::getOpenFileName(this, tr("Open DEM dataset"), 
								ui.DEM_DatasetLineEdit->text(), tr("Datasets (*.tif; *.tiff)"));
	if (!fileName.isNull())
	{
		if (mParams.DsDEM) GDALClose(mParams.DsDEM);
		mParams.DsDEM = GDALOpen(fileName.toStdString().c_str(), GA_ReadOnly);
		if (mParams.DsDEM)
		{
			GDALRasterBandH hrb = GDALGetRasterBand(mParams.DsDEM, 1);
			if (hrb)
			{
				int width = GDALGetRasterBandXSize(hrb);
				int height = GDALGetRasterBandYSize(hrb);

				ui.DEM_WidthLineEdit->setText(QString::number(width));
				ui.DEM_HeightLineEdit->setText(QString::number(height));

				ui.DEM_DatasetLineEdit->setText(fileName);
				refresh();
			}
		}
	}
}

void GenerateTilesTool::setTargetPath()
{
	mParams.TargetPath = QFileDialog::getExistingDirectory(this, tr("Select root path"), ui.targetPathLineEdit->text());
	ui.targetPathLineEdit->setText(mParams.TargetPath);
}

void GenerateTilesTool::refresh(const QString& str)
{
	mParams.PaddingMap = ui.mapPaddingLineEdit->text().toInt();
	mParams.PaddingDEM = ui.DEM_PaddingLineEdit->text().toInt();

	QSizeF dsSizeMap = QSizeF
	(
		ui.mapWidthLineEdit->text().toInt() - 2 * mParams.PaddingMap,
		ui.mapHeightLineEdit->text().toInt() - 2 * mParams.PaddingMap
	);
	
	QSizeF dsSizeDEM = QSizeF
	(
		ui.DEM_WidthLineEdit->text().toInt() - 2 * mParams.PaddingDEM,
		ui.DEM_HeightLineEdit->text().toInt() - 2 * mParams.PaddingDEM
	);
	
	QSizeF grSize = QSizeF
	(
		ui.tilesXLineEdit->text().toInt(),
		ui.tilesYLineEdit->text().toInt()
	);
	mParams.GridSize = grSize.toSize();
	
	QSizeF tileSizeMap = QSizeF
	(
		(dsSizeMap.width() - 2.0) / grSize.width(),
		(dsSizeMap.height() - 2.0) / grSize.height()
	);
	ui.mapTileWidthLineEdit->setText(QString::number(tileSizeMap.width()));	
	ui.mapTileHeightLineEdit->setText(QString::number(tileSizeMap.height()));

	QSizeF tileSizeDEM = QSizeF
	(
		(dsSizeDEM.width() - 4.0) / grSize.width(),
		(dsSizeDEM.height() - 4.0) / grSize.height()
	);
	ui.DEM_TileWidthLineEdit->setText(QString::number(tileSizeDEM.width()));	
	ui.DEM_TileHeightLineEdit->setText(QString::number(tileSizeDEM.height()));	

	
	QSize tileSizeIntMap = tileSizeMap.toSize();
	bool mapTileWidthEnabled = tileSizeMap.width() == tileSizeIntMap.width();
	bool mapTileHeightEnabled = tileSizeMap.height() == tileSizeIntMap.height();
	
	QPalette palette;
	palette.setColor(QPalette::Text, mapTileWidthEnabled ? Qt::black : Qt::red);
	ui.mapTileWidthLineEdit->setPalette(palette);
	palette.setColor(QPalette::Text, mapTileHeightEnabled ? Qt::black : Qt::red);
	ui.mapTileHeightLineEdit->setPalette(palette);

	QSize tileSizeIntDEM = tileSizeDEM.toSize();
	bool DEM_TileWidthEnabled = tileSizeDEM.width() == tileSizeIntDEM.width();
	bool DEM_TileHeightEnabled = tileSizeDEM.height() == tileSizeIntDEM.height();

	palette.setColor(QPalette::Text, DEM_TileWidthEnabled ? Qt::black : Qt::red);
	ui.DEM_TileWidthLineEdit->setPalette(palette);
	palette.setColor(QPalette::Text, DEM_TileHeightEnabled ? Qt::black : Qt::red);
	ui.DEM_TileHeightLineEdit->setPalette(palette);

	bool tileWidthEqual = tileSizeMap.width() == tileSizeDEM.width();
	bool tileHeightEqual = tileSizeMap.height() == tileSizeDEM.height();

	palette.setColor(QPalette::Text, tileWidthEqual ? Qt::black : Qt::red);
	ui.mapTileWidthLabel->setPalette(palette);
	ui.mapTileHeightLabel->setPalette(palette);
	
	palette.setColor(QPalette::Text, tileHeightEqual ? Qt::black : Qt::red);
	ui.DEM_TileWidthLabel->setPalette(palette);
	ui.DEM_TileHeightLabel->setPalette(palette);
	
	mParams.TileSize = tileSizeIntMap;

	bool enabled =	mapTileWidthEnabled && mapTileHeightEnabled &&
							DEM_TileWidthEnabled && DEM_TileHeightEnabled &&
							tileWidthEqual && tileHeightEqual;

	ui.runButton->setEnabled(enabled);
}

void GenerateTilesTool::start()
{
	mParams.GridOffset = QSize
	(
		ui.tileOffsetWidthLineEdit->text().toInt(),
		ui.tileOffsetHeightLineEdit->text().toInt()
	);
	mParams.Suffix = ui.tileSuffixLineEdit->text();

	mTilesGenerator.showProgress();
	emit generate(qVariantFromValue(mParams));
}