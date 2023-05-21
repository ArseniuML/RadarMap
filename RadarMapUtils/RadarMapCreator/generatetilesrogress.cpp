#include "generatetilesprogress.h"

GenerateTilesProgress::GenerateTilesProgress(QWidget* parent):
QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.cancelButton, SIGNAL(clicked()), this, SIGNAL(terminated()));
}

void GenerateTilesProgress::setProgress(int value)
{
	ui.progressBar->setValue(value);
}

