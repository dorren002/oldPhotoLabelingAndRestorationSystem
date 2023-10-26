#pragma once

#include <QWidget>
#include "ui_boxSlider.h"

class boxSlider : public QWidget
{
	Q_OBJECT

public:
	double value;

	boxSlider(QWidget *parent = nullptr);
	~boxSlider();
	void initValue(double val);
	void setMaxVal(double val);

private:
	Ui::boxSliderClass ui;

private slots:
	void slotDoubleSpinbox_slider();
	void slotslider_DoubleSpinBox();
};
