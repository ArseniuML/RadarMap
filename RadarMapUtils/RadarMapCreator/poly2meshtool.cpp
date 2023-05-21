#include "poly2meshtool.h"
#include "MeshProcessor.h"

#include <QFileDialog>
#include <QDirIterator>

Q_DECLARE_METATYPE(Poly2MeshParams);

MeshGenerator::MeshGenerator()
{
	connect(this, SIGNAL(progress(const QString&)), &mProgress, SLOT(setProgress(const QString&)), Qt::QueuedConnection);
	connect(this, SIGNAL(hideProgress()), &mProgress, SLOT(hide()), Qt::QueuedConnection);
	connect(&mProgress, SIGNAL(terminated()), this, SLOT(stopGenerating()), Qt::DirectConnection);
}

void MeshGenerator::showProgress()
{
	mProgress.setProgress(QString());
	mProgress.show();
}

void MeshGenerator::generate( QVariant params )
{
	Poly2MeshParams par = params.value<Poly2MeshParams>();

	mStopGenerating = false;

	QDir sourceDir(par.SourcePath);
	QDir targetDir(par.TargetPath);

	QDirIterator it(par.SourcePath, QStringList("*.flt"), QDir::Files, QDirIterator::Subdirectories);

	MeshProcessor meshProcessor;
		
	QString srcFilePath;
	QString tgtFilePath;

	while(it.hasNext() && !mStopGenerating)
	{
		it.next();
		QFileInfo srcInf = it.fileInfo();
					
		srcFilePath = srcInf.absoluteFilePath();
		QString relPath(sourceDir.relativeFilePath(srcInf.absolutePath()));
				
		QString fileName;
		if (relPath.isEmpty())
		{
			fileName = srcInf.fileName();
		}
		else
		{
			targetDir.mkpath(relPath);
			fileName = QDir(relPath).filePath(srcInf.fileName());
		}
		tgtFilePath = targetDir.filePath(fileName);
	
		meshProcessor.ProcessFlt(srcFilePath.toStdString().c_str(), tgtFilePath.toStdString().c_str());

		emit progress(fileName);
	}

	emit hideProgress();
}
	



Poly2MeshTool::Poly2MeshTool(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	mMeshGenerator.moveToThread(&mThread);
	connect(this, SIGNAL(generate(QVariant)), &mMeshGenerator, SLOT(generate(QVariant)), Qt::QueuedConnection);
	mThread.start();
}

Poly2MeshTool::~Poly2MeshTool()
{
	mMeshGenerator.stopGenerating();
	mThread.quit();
	mThread.wait();
}

void Poly2MeshTool::setSourcePath()
{
	mParams.SourcePath = QFileDialog::getExistingDirectory(this, tr("Select source path"), ui.sourcePathLineEdit->text());
	ui.sourcePathLineEdit->setText(mParams.SourcePath);
}

void Poly2MeshTool::setTargetPath()
{
	mParams.TargetPath = QFileDialog::getExistingDirectory(this, tr("Select target path"), ui.targetPathLineEdit->text());
	ui.targetPathLineEdit->setText(mParams.TargetPath);
}

void Poly2MeshTool::start()
{
	mMeshGenerator.showProgress();
	emit generate(qVariantFromValue(mParams));
}

