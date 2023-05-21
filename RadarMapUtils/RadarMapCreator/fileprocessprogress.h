#ifndef FILEPROCESSPROGRESS_H
#define FILEPROCESSPROGRESS_H

#include <QWidget>
#include "ui_fileprocessprogress.h"

class FileProcessProgress : public QWidget
{
	Q_OBJECT

public:
	FileProcessProgress(QWidget *parent = 0);
	~FileProcessProgress();
signals:
	void terminated();

public slots:
	void setProgress(const QString& value);

private:
	Ui::FileProcessProgressClass ui;
};

#endif // FILEPROCESSPROGRESS_H
