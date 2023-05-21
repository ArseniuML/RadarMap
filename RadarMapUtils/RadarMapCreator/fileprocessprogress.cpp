#include "fileprocessprogress.h"

FileProcessProgress::FileProcessProgress(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.cancelButton, SIGNAL(clicked()), this, SIGNAL(terminated()));
}

FileProcessProgress::~FileProcessProgress()
{

}

void FileProcessProgress::setProgress( const QString& value )
{
	ui.progressLineEdit->setText(value);
}
