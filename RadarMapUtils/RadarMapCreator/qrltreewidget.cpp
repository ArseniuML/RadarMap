#include "qrltreewidget.h"
#include "channelpropertiesdialog.h"

#include <QtGui/QFileDialog>
#include <QMenu>
#include <QMessageBox>

struct UserDataType
{
	enum Type
	{
		None,
		Gamma,
		LIA,
		MapChannel
	};
};

Q_DECLARE_METATYPE(GDALDatasetH)

QRLTreeWidget::QRLTreeWidget(QWidget *parent)
	: QTreeWidget(parent)
{
	mpSourcesItem = new QTreeWidgetItem();
	mpSourcesItem->setText(0, tr("Sources"));
	addTopLevelItem(mpSourcesItem);

	mpMapItem = new QTreeWidgetItem();
	mpMapItem->setText(0, tr("Map"));
	addTopLevelItem(mpMapItem);

	for (int i = 0; i < 2; ++i)
	{
		QTreeWidgetItem* item = new QTreeWidgetItem(mpMapItem, UserDataType::MapChannel);
		item->setText(0, tr("Channel ") + QString::number(i + 1));
	}

	expandItem(mpMapItem);
	
	connect(this, SIGNAL(currentItemChanged (QTreeWidgetItem*, QTreeWidgetItem*)),
		this, SLOT(currentItemChangedSlot(QTreeWidgetItem*, QTreeWidgetItem*)));
}

QRLTreeWidget::~QRLTreeWidget()
{
	closeSources();
	closeChannels();
}

void QRLTreeWidget::contextMenuEvent( QContextMenuEvent *event )
{
	QMenu menu;
	if (currentItem() == mpSourcesItem)
	{
		menu.addAction(tr("Add source"), this, SLOT(addSource()));
		menu.addAction(tr("Remove sources"), this, SLOT(closeSources()));
	}
	else if (currentItem() == mpMapItem)
	{
		menu.addAction(tr("Close map channels"), this, SLOT(closeChannels()));
	}
	
	if (currentItem()->data(0, Qt::UserRole).isNull())
	{
		if (currentItem()->type() == UserDataType::Gamma || 
			currentItem()->type() == UserDataType::LIA)
		{
			menu.addAction(tr("Open dataset"), this, SLOT(openDatasetReadonly()));
		}
		else if (currentItem()->type() == UserDataType::MapChannel)
		{
			menu.addAction(tr("Create dataset"), this, SLOT(createDataset()));
			menu.addAction(tr("Open dataset"), this, SLOT(openDatasetUpdate()));
		}
	}
	else if (currentItem()->type() == UserDataType::MapChannel)
	{
		menu.addAction(tr("Channel properties"), this, SLOT(setChannelProperties()));
	}
	
	menu.exec(cursor().pos());
}

void QRLTreeWidget::sourceData( std::vector<Source>& srcData )
{
	srcData.clear();
	for (int i = 0; i < mpSourcesItem->childCount(); ++i)
	{
		QTreeWidgetItem* sourceItem = mpSourcesItem->child(i);

		QVariant gamma = sourceItem->child(0)->data(0, Qt::UserRole);
		QVariant lia = sourceItem->child(1)->data(0, Qt::UserRole);

		if (!gamma.isNull() && !lia.isNull())
		{
			Source source;
			source.Gamma = GDALGetRasterBand(gamma.value<GDALDatasetH>(), 1);
			source.LIA = GDALGetRasterBand(lia.value<GDALDatasetH>(), 1);
			srcData.push_back(source);
		}
	}
}

void QRLTreeWidget::mapData( std::vector<Channel>& mapData )
{
	mapData.clear();
	for (int i = 0; i < mpMapItem->childCount(); ++i)
	{
		QTreeWidgetItem* channelItem = mpMapItem->child(i);
		QVariant channelVar = channelItem->data(0, Qt::UserRole);

		if (!channelVar.isNull())
		{
			GDALDatasetH ds = channelVar.value<GDALDatasetH>();

			Channel channel;
			channel.Raster = GDALGetRasterBand(ds, 1);
			channel.power = QString(GDALGetMetadataItem(ds, "TIFFTAG_IMAGEDESCRIPTION", NULL)).toFloat();
		
			mapData.push_back(channel);
		}
	}
}

void QRLTreeWidget::addSource()
{
	QTreeWidgetItem* sourceItem = new QTreeWidgetItem(currentItem());
	sourceItem->setText(0, tr("Source ") + QString::number(mpSourcesItem->childCount()));

	QTreeWidgetItem* gammaItem = new QTreeWidgetItem(sourceItem, UserDataType::Gamma);
	gammaItem->setText(0, tr("Gamma"));

	QTreeWidgetItem* liaItem = new QTreeWidgetItem(sourceItem, UserDataType::LIA);
	liaItem->setText(0, tr("LIA"));

	expandItem(mpSourcesItem);
	expandItem(sourceItem);
}

void QRLTreeWidget::closeSources()
{
	for (int i = mpSourcesItem->childCount() - 1; i >= 0; --i) 
	{
		QTreeWidgetItem* srcItem = mpSourcesItem->child(i);
		for (int j = srcItem->childCount() -1; j >=0; --j)
		{
			QTreeWidgetItem* item = srcItem->child(j);
			QVariant v = item->data(0, Qt::UserRole);
			if (!v.isNull())
			{
				GDALClose(v.value<GDALDatasetH>());
			}
			delete item;
		}
		delete srcItem;
	}
}

void QRLTreeWidget::closeChannels()
{
	for(int i = mpMapItem->childCount() - 1; i >= 0; --i) 
	{
		QTreeWidgetItem* item = mpMapItem->child(i);
		QVariant v = item->data(0, Qt::UserRole);
		if (!v.isNull())
		{
			GDALClose(v.value<GDALDatasetH>());
			item->setData(0, Qt::UserRole, QVariant());
		}
	}
}

void QRLTreeWidget::setChannelProperties()
{
	GDALDatasetH ds = currentItem()->data(0, Qt::UserRole).value<GDALDatasetH>();
	QString str(GDALGetMetadataItem(ds, "TIFFTAG_IMAGEDESCRIPTION", NULL));

	double power = 1;
	if (!str.isEmpty())
	{
		power = str.toDouble();
	}

	ChannelPropertiesDialog dlg;
	dlg.setPower(power);

	if (dlg.exec() == QDialog::Accepted)
	{
		power = dlg.power();
		GDALSetMetadataItem(ds, "TIFFTAG_IMAGEDESCRIPTION", 
			QString::number(power).toStdString().c_str(), NULL);
	}
}

void QRLTreeWidget::createDataset()
{
	QString fileName = 
		QFileDialog::getSaveFileName(this, tr("New map channel"), QDir::currentPath(), tr("Datasets (*.tif; *.tiff)"));

	if (!fileName.isEmpty() && !mImageSize.isEmpty()) 
	{
		GDALDatasetH ds = GDALCreate
		(
			GDALGetDriverByName("GTiff"),
			fileName.toStdString().c_str(), 
			mImageSize.width(), 
			mImageSize.height(), 
			1, 
			GDT_Float32,
			NULL
		);

		if (ds)
		{
			GDALSetMetadataItem(ds, "TIFFTAG_IMAGEDESCRIPTION", "1", NULL);

			currentItem()->setData(0, Qt::UserRole, qVariantFromValue(ds));
			emit datasetSelected(ds);
		}
		else
		{
			QMessageBox::warning
			(
				this, 
				tr("Radar map creator"), 
				tr("Can't create ") + fileName, 
				QMessageBox::Ok
			);
		}
	}
}

void QRLTreeWidget::openDataset(GDALAccess access)
{
	QString fileName = 
		QFileDialog::getOpenFileName(this, tr("Open ") + currentItem()->text(0), 
		QDir::currentPath(), tr("Datasets (*.tif; *.tiff)"));

	if (!fileName.isEmpty()) 
	{
		GDALDatasetH ds = GDALOpen(fileName.toStdString().c_str(), access);
		if (ds)
		{
			mImageSize.setWidth(GDALGetRasterXSize(ds));
			mImageSize.setHeight(GDALGetRasterYSize(ds));
			
			currentItem()->setData(0, Qt::UserRole, qVariantFromValue(ds));
			emit datasetSelected(ds);
		}
		else
		{
			QMessageBox::warning
			(
				this, 
				tr("Radar map creator"), 
				tr("Can't open ") + fileName, 
				QMessageBox::Ok
			);
		}
	}
}

void QRLTreeWidget::currentItemChangedSlot( QTreeWidgetItem * current, QTreeWidgetItem * previous )
{
	GDALDatasetH ds = NULL;
	if (current)
	{
		QVariant v = current->data(0, Qt::UserRole);
		if (!v.isNull())
		{
			ds = v.value<GDALDatasetH>();
		}
	}
	emit datasetSelected(ds);
}

