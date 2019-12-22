#include "vize/ui/sample_step_widget.hpp"
#include "ui_sample_step_widget.h"

#include "vize/painter/volume_raycaster.hpp"

namespace vize {

SampleStepWidget::SampleStepWidget(VolumeRaycaster& raycaster, QWidget* parent)
	: QWidget(parent), _raycaster(raycaster), ui(std::make_unique<Ui::SampleStepWidget>())
{
	ui->setupUi(this);

	connect(ui->doubleSpinBox_sampleStep, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &SampleStepWidget::_doubleSpinBox_sampleStep_valueChanged);
}

SampleStepWidget::~SampleStepWidget() { }

Float SampleStepWidget::getSampleStep() const {
	return Float(ui->doubleSpinBox_sampleStep->value());
}
void  SampleStepWidget::setSampleStep(Float step) {
	ui->doubleSpinBox_sampleStep->setValue(double(step));
}
void SampleStepWidget::_doubleSpinBox_sampleStep_valueChanged(double) const {
	_raycaster.setSampleStep(getSampleStep());
}

}