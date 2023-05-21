#ifndef GENERATETILES_H
#define GENERATETILES_H

#include <QWidget>
#include "ui_GenerateTilesProgress.h"

class GenerateTilesProgress : public QWidget
{
	Q_OBJECT

public:
	GenerateTilesProgress(QWidget* parent = NULL);

signals:
	void terminated();

	public slots:
	void setProgress(int value);

private:
	Ui::GenerateTilesProgressClass ui;
};

#endif // GENERATETILES_H
