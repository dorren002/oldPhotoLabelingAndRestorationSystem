#pragma once

#include <QWidget>
#include "ui_boxSlider.h"

class boxSlider : public QWidget
{
	Q_OBJECT

public:
	boxSlider(QWidget *parent = nullptr);
	~boxSlider();

	void setValue(double val);
	void setMaxVal(double val);
	void setStep(double val);

	double getValue();

private:
	double thval;
	double RATIO;

	Ui::boxSliderClass ui;

private slots:
	void slotDoubleSpinbox_slider();
	void slotslider_DoubleSpinBox();
};
