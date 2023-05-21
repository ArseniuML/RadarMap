#include "channelpropertiesdialog.h"
#include "constants.h"

#include <QPen>
#include <qwt_text.h>
#include <qwt_plot_grid.h>

ChannelPropertiesDialog::ChannelPropertiesDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	QwtText title("Reflection curve");
	title.setFont(QFont("Verdana", 10, QFont::Normal));
	ui.qwtPlot->setTitle(title);

	QwtPlotGrid* grid = new QwtPlotGrid();
	grid->enableXMin(true);
	grid->enableYMin(true);
	grid->setMajPen(QPen(Qt::black, 0, Qt::DotLine));
	grid->setMinPen(QPen(Qt::gray,0, Qt::DotLine));
	grid->attach(ui.qwtPlot);

	QwtText xAxisTitle("Incidence angle, degrees"); 
	xAxisTitle.setFont(QFont("Verdana", 10, QFont::Normal));
	ui.qwtPlot->setAxisTitle(QwtPlot::xBottom, xAxisTitle);
	ui.qwtPlot->setAxisScale(QwtPlot::xBottom, 0, 90);

	QwtText yAxisTitle("Gamma");
	yAxisTitle.setFont(QFont("Verdana", 10, QFont::Normal));
	ui.qwtPlot->setAxisTitle(QwtPlot::yLeft, yAxisTitle);
	ui.qwtPlot->setAxisScale(QwtPlot::yLeft, 0, 1);

	mpCurve = new QwtPlotCurve();
	mpCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
	mpCurve->setPen(QPen(Qt::blue));
	mpCurve->attach(ui.qwtPlot);

	ui.Slider->setRange(-4, 4, 0.01);
	ui.Slider->setValue(0);
}

ChannelPropertiesDialog::~ChannelPropertiesDialog()
{

}

void ChannelPropertiesDialog::setPower( double power, bool updateSlider)
{
	mPower = power;
	ui.powerLineEdit->setText(QString::number(power));
	if (updateSlider)
	{
		ui.Slider->setValue(-log(power)/log(double(4)));
	}
	replot();
}

void ChannelPropertiesDialog::sliderValueChanged( double value )
{
	setPower(pow(4, -value), false);
}

double ChannelPropertiesDialog::power()
{
	return mPower;
}

void ChannelPropertiesDialog::replot()
{
	int crvSamples = 500;
	std::vector<double> curveX(crvSamples), curveY(crvSamples);
	for (int i = 0; i < crvSamples; ++i)
	{
		double rad = 0.5 * PI * double(i) / double(crvSamples - 1);
		double cosPt = cos(rad);

		curveX[i] = 180.0 * rad / PI;
		curveY[i] = pow(cosPt, mPower);
	}

	mpCurve->setSamples(&curveX[0], &curveY[0], crvSamples);
	ui.qwtPlot->replot();
}


