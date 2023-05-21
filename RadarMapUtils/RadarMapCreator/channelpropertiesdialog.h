#ifndef CHANNELPROPERTIESDIALOG_H
#define CHANNELPROPERTIESDIALOG_H

#include <QDialog>
#include "ui_ChannelPropertiesDialog.h"

#include <qwt_plot_curve.h>

class ChannelPropertiesDialog : public QDialog
{
	Q_OBJECT

public:
	ChannelPropertiesDialog(QWidget *parent = (QWidget*) 0);
	~ChannelPropertiesDialog();

	void setPower(double power, bool updateSlider = true);
	double power();

private:
	void replot();
		
private slots:
	void sliderValueChanged(double value);

private:
	Ui::ChannelPropertiesDialogClass ui;

	QwtPlotCurve* mpCurve;
	double mPower;
};

#endif // CHANNELPROPERTIESDIALOG_H
