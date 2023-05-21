#ifndef QRLTREEWIDGET_H
#define QRLTREEWIDGET_H

#include <QTreeWidget>
#include <vector>
#include <gdal_priv.h>

struct Source
{
	GDALRasterBandH Gamma;
	GDALRasterBandH LIA;
};

struct Channel
{
	GDALRasterBandH Raster;
	float power;
};

class QRLTreeWidget : public QTreeWidget
{
	Q_OBJECT

public:
	QRLTreeWidget(QWidget *parent);
	~QRLTreeWidget();

	void contextMenuEvent(QContextMenuEvent *event);
	
	void sourceData(std::vector<Source>& srcData);
	void mapData(std::vector<Channel>& mapData);

	const QSize& imageSize() {return mImageSize;}

signals:
	void datasetSelected(GDALDatasetH ds);

private slots:
	void addSource();
	void closeSources();
	void closeChannels();

	void createDataset();

	void openDatasetReadonly() {openDataset(GA_ReadOnly);}
	void openDatasetUpdate() {openDataset(GA_Update);}
	void openDataset(GDALAccess access);

	void setChannelProperties();

	void currentItemChangedSlot(QTreeWidgetItem * current, QTreeWidgetItem * previous);

private:
	QTreeWidgetItem* mpSourcesItem;
	QTreeWidgetItem* mpMapItem;

	QSize mImageSize;
};

#endif // QRLTREEWIDGET_H
