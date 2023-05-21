#ifndef PIXELINSPECTOR_H
#define PIXELINSPECTOR_H

#include <QWidget>
#include "Ui_PixelInspector.h"

#include <qwt_plot_curve.h>

class PixelInspector : public QWidget
{
	Q_OBJECT

public:
	PixelInspector(QWidget *parent = 0);
	~PixelInspector();

	void Inspect(const std::vector<double>& LIA,
		const std::vector<double>& gamma,
		const std::vector<double>& powers,
		const std::vector<double>& channels);

private:
	Ui::PixelInspectorClass ui;
	
	QwtPlotCurve* mpScatterCurve;
	QwtPlotCurve* mpReconstructedCurve;
};

#endif // PIXELINSPECTOR_H
