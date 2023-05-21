#ifndef GENERATEMAP_H
#define GENERATEMAP_H

#include <QWidget>
#include "ui_GenerateMapProgress.h"

#include <QThread>
#include "radarmapcreator.h"

class GenerateMapThread : public QThread
{
	Q_OBJECT

public:
	GenerateMapThread(RadarMapCreator* creator);
	~GenerateMapThread();

	void run();

private:
	RadarMapCreator* mpCreator;
};

class GenerateMapProgress : public QWidget
{
	Q_OBJECT

public:
	GenerateMapProgress(RadarMapCreator *parent);
	~GenerateMapProgress();

private:
	Ui::GenerateMapProgressClass ui;

	RadarMapCreator* mpCreator;
	GenerateMapThread mThread;

private slots:
	void start();
	void displayProfile();
};

#endif // GENERATEMAP_H
