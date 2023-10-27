#include "boxSlider.h"
#include "qdebug.h"

boxSlider::boxSlider(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	thval = 0;
	RATIO = 100;
	
	connect(ui.doubleSpinBox, SIGNAL(valueChanged()), this, SLOT(slotDoubleSpinbox_slider()));
	connect(ui.horizontalSlider, SIGNAL(valueChanged()), this, SLOT(slotslider_DoubleSpinBox()));
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
	qDebug() << "spin to slider";
	thval = ui.doubleSpinBox->value();
	ui.horizontalSlider->setValue((int)(thval));
}

void boxSlider::slotslider_DoubleSpinBox()
{
	qDebug() << "slider to spin";
	thval = ui.horizontalSlider->value() / RATIO;
	ui.doubleSpinBox->setValue((double)(thval * RATIO));
}