#include "generatemapprogress.h"

GenerateMapThread::GenerateMapThread( RadarMapCreator* creator ) 
	:QThread()
{
	mpCreator = creator;
}

GenerateMapThread::~GenerateMapThread()
{

}

void GenerateMapThread::run()
{
	mpCreator->generateMapProc();
}

GenerateMapProgress::GenerateMapProgress(RadarMapCreator *parent)
	: QWidget(parent, Qt::Dialog),
	mpCreator(parent),
	mThread(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	
	connect(&mThread, SIGNAL(finished()), this, SLOT(displayProfile()));
	connect(mpCreator, SIGNAL(setGenerateMapProgress(int)), 
		ui.progressBar, SLOT(setValue(int)));
	
	ui.numThreadsLineEdit->setText(QString::number(mpCreator->mNumThreads));
	show();
}

GenerateMapProgress::~GenerateMapProgress()
{
	mpCreator->stopMapGenerating();
	mThread.wait();
}

void GenerateMapProgress::start()
{
	mpCreator->mNumThreads = ui.numThreadsLineEdit->text().toInt();
	mThread.start();
}

void GenerateMapProgress::displayProfile()
{
	ui.deltaTimeLineEdit->setText(QString::number(mpCreator->mDeltaTime));
	ui.numThreadsLineEdit->setText(QString::number(mpCreator->mNumThreads));
}
