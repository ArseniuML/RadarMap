#ifndef POLY2MESHTOOL_H
#define POLY2MESHTOOL_H

#include <QDialog>
#include <QThread>

#include "ui_poly2meshtool.h"
#include "fileprocessprogress.h"

struct Poly2MeshParams
{
	QString SourcePath;
	QString TargetPath;
};

class MeshGenerator : public QObject
{
	Q_OBJECT

public:
	MeshGenerator();
	void showProgress();

signals:
	void hideProgress();
	void progress(const QString& value);

public slots:
	void generate(QVariant params);
	void stopGenerating() {mStopGenerating = true;}

private:
	volatile bool mStopGenerating;
	FileProcessProgress mProgress;
};

class Poly2MeshTool : public QDialog
{
	Q_OBJECT

public:
	Poly2MeshTool(QWidget *parent = 0);
	~Poly2MeshTool();

signals:
	void generate(QVariant params);

private slots:
	void setSourcePath();
	void setTargetPath();

	void start();

private:
	Ui::Poly2MeshToolClass ui;
	Poly2MeshParams mParams;
	
	QThread mThread;
	MeshGenerator mMeshGenerator;
};

#endif // POLY2MESHTOOL_H
