#ifndef QRLGRAPHICSVIEW_H
#define QRLGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsPixmapItem>

#include <vector>
#include <gdal_priv.h>

class QRLGraphicsView : public QGraphicsView
{
	Q_OBJECT

public:
	QRLGraphicsView(QWidget *parent = 0);
	~QRLGraphicsView();

	void setRange(float min, float max);
	void setBrightness(float brightness);

signals:
	void pixelSelected(const QPoint& point);

public slots:
	void setSource(GDALDatasetH hDataset);

protected:
	void readSource();
	void floatTobyte();
	
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	
	void wheelEvent ( QWheelEvent * event ) {};
	bool eventFilter(QObject *object, QEvent *event);

	QGraphicsScene mScene;
	QGraphicsPixmapItem mPixmapItem;

	GDALRasterBandH mhRasterBand;
	
	std::vector<float> mBufferFloat;
	std::vector<uchar> mBufferByte;

	float mMin, mMax;
	float mBrightness;

	QPoint mPanPoint;
};

#endif // QRLGRAPHICSVIEW_H
