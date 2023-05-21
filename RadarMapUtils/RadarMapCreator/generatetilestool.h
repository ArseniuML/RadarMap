#ifndef GENERATETILESDIALOG_H
#define GENERATETILESDIALOG_H

#include <QWidget>
#include <QThread>

#include "ui_GenerateTilesTool.h"
#include "generatetilesprogress.h"

#include <gdal_priv.h>

#define __NO_STD_VECTOR
#define __CL_ENABLE_EXCEPTIONS // needed for exceptions
#include <CL/cl.hpp>

struct GenerateTilesParams
{
	GenerateTilesParams()
	{
		DsMap = NULL;
		DsDEM = NULL;
	}

	GDALDatasetH DsMap;
	GDALDatasetH DsDEM;

	QSize GridSize;
	QSize GridOffset;
	QSize TileSize;
	
	int PaddingMap;
	int PaddingDEM;

	QString Suffix;
	QString TargetPath;
};

class TilesGenerator : public QObject
{
	Q_OBJECT

public:
	TilesGenerator();
	void showProgress();

signals:
	void hideProgress();
	void progress(int value);

public slots:
	void init();
	void generate(QVariant params);
	void stopGenerating() {mStopGenerating = true;}
	void destroy();

private:
	cl::Context* mpclContext;
	cl::CommandQueue* mpclCommandQueue;
	cl::Kernel* mpclKernel;

	volatile bool mStopGenerating;
	GenerateTilesProgress mProgress;
};


class GenerateTilesTool : public QDialog
{
	Q_OBJECT

public:
	GenerateTilesTool(QWidget *parent = (QWidget*) 0);
	~GenerateTilesTool();

signals:
	void generate(QVariant params);
	void destroyGenerator();

private slots:
	void openMapDataset();
	void openDEM_Dataset();
	void setTargetPath();
	
	void refresh(const QString& str = QString());
	void start();

private:
	Ui::GenerateTilesToolClass ui;
	GenerateTilesParams mParams;

	QThread mThread;
	TilesGenerator mTilesGenerator;
};

#endif // GENERATETILESDIALOG_H
