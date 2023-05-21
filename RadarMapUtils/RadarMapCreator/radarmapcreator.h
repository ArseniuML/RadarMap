#ifndef RADARMAPCREATOR_H
#define RADARMAPCREATOR_H

#include <QtGui/QMainWindow>
#include "ui_radarmapcreator.h"

#include "pixelinspector.h"

class RadarMapCreator : public QMainWindow
{
	Q_OBJECT

public:
	RadarMapCreator(QWidget *parent = 0, Qt::WFlags flags = 0);
	~RadarMapCreator();

	void generateMapProc();
	void stopMapGenerating() {mStopMapGenerating = true;}

signals:
	void setGenerateMapProgress(int value);

private slots:
	void generateMap();
	void generateTiles();
	void poly2Mesh();

	void showPixelInspector() {mpPixelInspector->show();}
	void inspectPixel(const QPoint& point);
	
private:
	Ui::RadarMapCreatorClass ui;

	PixelInspector* mpPixelInspector;

	QString mRootTilesPath;
	int mPadding;
	QSize mGridSize, mTileOffset;
	QString mTileSuffix;

	volatile bool mStopMapGenerating;

public:
	int mNumThreads;
	double mDeltaTime;
};

#endif // RADARMAPCREATOR_H
