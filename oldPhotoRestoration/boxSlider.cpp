#include "boxSlider.h"

boxSlider::boxSlider(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	
	connect(ui.doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slotDoubleSpinbox_slider()));
	connect(ui.horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(slotslider_DoubleSpinBox()));
}

boxSlider::~boxSlider()
{}

void boxSlider::setMaxVal(double val) {
	ui.doubleSpinBox->setRange(0.0, val);
	ui.horizontalSlider->setRange(0.0, val);
}

void boxSlider::initValue(double val) {
	ui.horizontalSlider->setValue(val);
	//ui.doubleSpinBox->setValue(val);
}

void boxSlider::slotDoubleSpinbox_slider()
{
	ui.horizontalSlider->setValue((int)(ui.doubleSpinBox->value()));
}

void boxSlider::slotslider_DoubleSpinBox()
{
	ui.doubleSpinBox->setValue((double)(ui.horizontalSlider->value()));
}