#include "boxSlider.h"
#include "qdebug.h"

boxSlider::boxSlider(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	thval = 0;
	RATIO = 100;
	
	connect(ui.doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slotDoubleSpinbox_slider()));
	connect(ui.horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(slotslider_DoubleSpinBox()));
}

boxSlider::~boxSlider()
{}

void boxSlider::setMaxVal(double val) {
	ui.doubleSpinBox->setRange(0.0, val);
	ui.horizontalSlider->setRange(0.0, val * RATIO);
}

void boxSlider::setValue(double val) {
	ui.horizontalSlider->setValue(val * RATIO);
	ui.doubleSpinBox->setValue(val);
}

double boxSlider::getValue() {
	return thval;
}

void boxSlider::setStep(double value) {
	ui.doubleSpinBox->setSingleStep(value);
}

void boxSlider::slotDoubleSpinbox_slider()
{
	thval = ui.doubleSpinBox->value();
	ui.horizontalSlider->setValue((int)(thval * RATIO));
}

void boxSlider::slotslider_DoubleSpinBox()
{
	thval = ui.horizontalSlider->value() / RATIO;
	ui.doubleSpinBox->setValue((double)(thval));
}