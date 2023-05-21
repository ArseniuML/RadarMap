#include "radarmapcreator.h"
#include "constants.h"

#include "generatetilestool.h"
#include "poly2meshtool.h"

#include "leastsquaressolver.h"
#include "generatemapprogress.h"

#include <vector>
#include <gdal_priv.h>

#define API_LEV2
#include <mgapiall.h>

#include <omp.h>

RadarMapCreator::RadarMapCreator(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	setFixedSize(size());
	setWindowFlags((windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowMaximizeButtonHint);

	mpPixelInspector = new PixelInspector(this);
	
	mNumThreads = 0;
	mDeltaTime = 0;
	
	ui.treeWidget->setHeaderLabel("Datasets");

	connect(ui.treeWidget, SIGNAL(datasetSelected(GDALDatasetH)), 
		ui.graphicsView, SLOT(setSource(GDALDatasetH)));
	connect(ui.graphicsView, SIGNAL(pixelSelected(QPoint)), this, SLOT(inspectPixel(QPoint)));
	
	GDALAllRegister();
	mgInit(NULL, NULL);
}

RadarMapCreator::~RadarMapCreator()
{
	mgExit();
}

void RadarMapCreator::generateMap()
{
	mStopMapGenerating = false;
	new GenerateMapProgress(this);
}

void RadarMapCreator::generateTiles()
{
	GenerateTilesTool tool(this);
	tool.exec();
}

void RadarMapCreator::poly2Mesh()
{
	Poly2MeshTool tool(this);
	tool.exec();
}

void RadarMapCreator::generateMapProc()
{
	double startTime = omp_get_wtime();

	std::vector<Source> sources;
	ui.treeWidget->sourceData(sources);
	
	std::vector<Channel> channels;
	ui.treeWidget->mapData(channels);

	int nSources = sources.size();
	int nChannels = channels.size();

	if (nSources > 0 && nChannels > 0)
	{
		std::vector<std::vector<float>> srcLIA(nSources), srcGamma(nSources);
		std::vector<std::vector<float>> map(nChannels);
		
		std::vector<float> powers(nChannels);
		for (int i = 0; i < nChannels; ++i)
		{
			powers[i] = channels[i].power;
		}
		
		int count = 0;
		bool stop = false;

		QSize imageSize = ui.treeWidget->imageSize();
		
		if (mNumThreads > 0)
		{
			omp_set_num_threads(mNumThreads);
		}

		#pragma omp parallel
		{
			int threadId = omp_get_thread_num();
			int nThreads = omp_get_num_threads();
			int tail = imageSize.height() % nThreads;

			LeastSquaresSolver lss;
			lss.Initialize(nSources, nChannels);
			
			#pragma omp master
			{
				mNumThreads = nThreads;

				for (int i = 0; i < nSources; ++i)
				{
					srcLIA[i].resize(imageSize.width() * nThreads);
					srcGamma[i].resize(imageSize.width() * nThreads);
				}

				for (int i = 0; i < nChannels; ++i)
				{
					map[i].resize(imageSize.width() * nThreads);
				}
			}

			#pragma omp barrier

			for(int row = 0; row < imageSize.height(); row += nThreads)
			{
				int nActiveThreads = imageSize.height() - row > tail ? nThreads : tail;

				#pragma omp master
				{
					for (int src = 0; src < nSources; ++src)
					{
						GDALRasterIO
						(
							sources[src].Gamma, GF_Read, 
							0, row, 
							imageSize.width(), nActiveThreads,
							&srcGamma[src][0],
							imageSize.width(), nActiveThreads,
							GDT_Float32,	0,	0
						);

						GDALRasterIO
						(
							sources[src].LIA, GF_Read, 
							0, row, 
							imageSize.width(), nActiveThreads,
							&srcLIA[src][0],
							imageSize.width(), nActiveThreads,
							GDT_Float32,	0,	0
						);
					}
				}

				#pragma omp barrier
				
				if (stop) break;

				if (threadId < nActiveThreads)
				{
					lss.Solve(srcLIA, srcGamma, map, powers, imageSize.width(), threadId * imageSize.width());
					
					#pragma omp atomic
					++count;
				}

				#pragma omp barrier
				
				#pragma omp master
				{
					for (int tgt = 0; tgt < nChannels; ++tgt)
					{
						GDALRasterIO
						(
							channels[tgt].Raster, GF_Write,
							0, row, 
							imageSize.width(), nActiveThreads,
							&map[tgt][0],
							imageSize.width(), nActiveThreads,
							GDT_Float32,	0,	0
						);		
					}

					emit setGenerateMapProgress(100.0 * double(count) / double(imageSize.height()));

					if (mStopMapGenerating) stop = true;
				}
			}
		}
	}

	mDeltaTime = omp_get_wtime() - startTime;
}

void RadarMapCreator::inspectPixel( const QPoint& point )
{
	if (!mpPixelInspector->isHidden())
	{
		std::vector<Source> sources;
		ui.treeWidget->sourceData(sources);
	
		std::vector<Channel> channels;
		ui.treeWidget->mapData(channels);
	
		int nSources = sources.size();
		std::vector<double> LIA(nSources), gamma(nSources);
	
		for (int i = 0; i < nSources; ++i)
		{
			float tmp;
			GDALRasterIO
			(
				sources[i].LIA, GF_Read, 
				point.x(), point.y(), 1, 1,
				&tmp,
				1, 1,
				GDT_Float32,	0,	0
			);

			LIA[i] = abs(tmp);

			GDALRasterIO
			(
				sources[i].Gamma, GF_Read, 
				point.x(), point.y(), 1, 1,
				&tmp,
				1, 1,
				GDT_Float32,	0,	0
			);
			gamma[i] = tmp;
		}

		int nChannels = channels.size();
		std::vector<double> map(nChannels), powers(nChannels);

		for (int i = 0; i < nChannels; ++i)
		{
			float tmp;
			GDALRasterIO
			(
				channels[i].Raster,
				GF_Read, point.x(), point.y(), 1, 1,
				&tmp,
				1, 1,
				GDT_Float32,	0,	0
			);
			map[i] = tmp;
			powers[i] = channels[i].power;
		}

		mpPixelInspector->Inspect(LIA, gamma, powers, map);
	}
}
