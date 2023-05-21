#include "pixelinspector.h"

#include <qwt_plot_grid.h>
#include <qwt_symbol.h>

#include "radarmapcreator.h"
#include "Constants.h"

PixelInspector::PixelInspector(QWidget *parent)
	: QWidget(parent, Qt::Tool)
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

	mpScatterCurve = new QwtPlotCurve();
	mpScatterCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
	mpScatterCurve->setStyle(QwtPlotCurve::NoCurve);
	
	QwtSymbol* pSymbol = new QwtSymbol();
	pSymbol->setStyle(QwtSymbol::Ellipse);
	pSymbol->setPen(QColor(Qt::black));
	pSymbol->setSize(4);
	mpScatterCurve->setSymbol(pSymbol);
	mpScatterCurve->attach(ui.qwtPlot);
	
	mpReconstructedCurve = new QwtPlotCurve();
	mpReconstructedCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
	mpReconstructedCurve->setPen(QPen(Qt::blue));
	mpReconstructedCurve->attach(ui.qwtPlot);
}

PixelInspector::~PixelInspector()
{
}

void PixelInspector::Inspect( const std::vector<double>& LIA, 
	const std::vector<double>& gamma, 
	const std::vector<double>& powers, 
	const std::vector<double>& channels )
{
	if (LIA.size() == gamma.size())
		mpScatterCurve->setSamples(&LIA[0], &gamma[0], LIA.size());

	if (powers.size() == channels.size())
	{
		int crvSamples = 500;
		std::vector<double> curveX(crvSamples), curveY(crvSamples);
		for (int i = 0; i < crvSamples; ++i)
		{
			double rad = 0.5 * PI * double(i) / double(crvSamples - 1);
			double cosPt = cos(rad);

			curveX[i] = 180.0 * rad / PI;

			for (int j = 0; j < powers.size(); ++j)
			{
				curveY[i] = curveY[i] + 10.0 * channels[j] * pow(cosPt, powers[j]);
			}
		}
		mpReconstructedCurve->setSamples(&curveX[0], &curveY[0], crvSamples);
	}
	
	ui.qwtPlot->replot();
}
