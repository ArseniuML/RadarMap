#include "qrlgraphicsview.h"
#include <QScrollBar>

QRLGraphicsView::QRLGraphicsView(QWidget *parent)
	: QGraphicsView(parent)
{
	mhRasterBand = NULL;
	
	mMin = 0;
	mMax = 0.2;
	mBrightness = 1;

	mScene.addItem(&mPixmapItem);
}

QRLGraphicsView::~QRLGraphicsView()
{
	
}

void QRLGraphicsView::setSource( GDALDatasetH hDataset )
{
	if (hDataset)
	{
		GDALRasterBandH hRasterBand = GDALGetRasterBand(hDataset, 1);
		if (hRasterBand)
		{
			setScene(&mScene);
			setSceneRect(0, 0, GDALGetRasterBandXSize(hRasterBand), GDALGetRasterBandYSize(hRasterBand));
			if (!mhRasterBand) centerOn(sceneRect().center());

			if (horizontalScrollBar()) horizontalScrollBar()->installEventFilter(this);
			if (verticalScrollBar()) verticalScrollBar()->installEventFilter(this);

			mhRasterBand = hRasterBand;
			readSource();
			return;
		}
	}

	setScene(NULL);
	setSceneRect(QRectF());
	mhRasterBand = NULL;
}

void QRLGraphicsView::setRange( float min, float max )
{
	mMin = min;
	mMax = max;

	if (mBufferByte.size() > 0)
	{
		floatTobyte();

		QImage im
		(
			&mBufferByte[0], 
			mPixmapItem.pixmap().width(), 
			mPixmapItem.pixmap().height(), 
			QImage::Format_RGB32
		);
		mPixmapItem.setPixmap(QPixmap::fromImage(im));
	}
}

void QRLGraphicsView::setBrightness( float brightness )
{
	mBrightness = brightness;

	if (mBufferByte.size() > 0)
	{
		floatTobyte();

		QImage im
		(
			&mBufferByte[0], 
			mPixmapItem.pixmap().width(), 
			mPixmapItem.pixmap().height(), 
			QImage::Format_RGB32
		);
		mPixmapItem.setPixmap(QPixmap::fromImage(im));
	}
}

void QRLGraphicsView::readSource()
{
	if (mhRasterBand)
	{
		QRect sourceRect = (mapToScene(viewport()->rect()).boundingRect() & sceneRect()).toRect();

		mBufferFloat.resize(sourceRect.width() * sourceRect.height());
		mBufferByte.resize(4 * mBufferFloat.size());

		CPLErr err = GDALRasterIO
		(
			mhRasterBand,
			GF_Read,
			sourceRect.left(),
			sourceRect.top(),
			sourceRect.width(), 
			sourceRect.height(),
			&mBufferFloat[0],
			sourceRect.width(), 
			sourceRect.height(),
			GDT_Float32,
			0,
			0
		);

		//gdalgetdata
		floatTobyte();

		mPixmapItem.hide();
		QImage im(&mBufferByte[0], sourceRect.width(), sourceRect.height(), QImage::Format_RGB32);
		mPixmapItem.setPixmap(QPixmap::fromImage(im));
		mPixmapItem.setPos(sourceRect.topLeft());
		mPixmapItem.show();
	}
}


void QRLGraphicsView::floatTobyte()
{
	for (int i = 0, j = 0; i < mBufferFloat.size(); ++i)
	{
		float pix = mBrightness * (mBufferFloat[i] - mMin) / (mMax - mMin);
		if (pix < 0) pix = 0;	
		if (pix > 1) pix = 1;

		uchar pixByte = (uchar)(pix * 255);
		mBufferByte[j++] = pixByte;
		mBufferByte[j++] = pixByte;
		mBufferByte[j++] = pixByte;
		mBufferByte[j++] = 255;
	}
}

void QRLGraphicsView::mousePressEvent( QMouseEvent* event )
{

	if (event->button() == Qt::LeftButton)
	{
		mPanPoint = event->pos();
		setCursor(Qt::ClosedHandCursor);
	}
	else if (event->button() == Qt::RightButton)
	{
		emit pixelSelected(mapToScene(event->pos()).toPoint());
	}	
}

void QRLGraphicsView::mouseReleaseEvent( QMouseEvent *event )
{
	if (event->button() == Qt::LeftButton)
	{
		setCursor(Qt::ArrowCursor);
		mPanPoint = QPoint();
		readSource();
	}
}

void QRLGraphicsView::mouseMoveEvent( QMouseEvent *event )
{
	if(!mPanPoint.isNull()) 
	{
		QPointF delta = mapToScene(mPanPoint) - mapToScene(event->pos());
		mPanPoint = event->pos();
		centerOn(mapToScene(viewport()->rect().center()) + QPointF(1.0, 1.0) + delta);
	}
}

bool QRLGraphicsView::eventFilter( QObject *object, QEvent *event )
{
	if ((object == horizontalScrollBar() || object == verticalScrollBar())
		&& event->type() == QEvent::MouseButtonRelease)
	{
		readSource();
	}
	return false;
}


