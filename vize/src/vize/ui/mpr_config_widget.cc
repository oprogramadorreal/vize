#include "vize/ui/mpr_config_widget.hpp"
#include "ui_mpr_config_widget.h"

#include "vize/painter/volume_mpr.hpp"

namespace vize {

MPRConfigWidget::MPRConfigWidget(VolumeMPR& mpr, QWidget* parent)
	: QWidget(parent), _mpr(mpr), ui(std::make_unique<Ui::MPRConfigWidget>())
{
	ui->setupUi(this);

	connect(ui->horizontalSlider_xPlane, &QSlider::valueChanged, this, &MPRConfigWidget::_horizontalSlider_xPlane_valueChanged);
	connect(ui->horizontalSlider_yPlane, &QSlider::valueChanged, this, &MPRConfigWidget::_horizontalSlider_yPlane_valueChanged);
	connect(ui->horizontalSlider_zPlane, &QSlider::valueChanged, this, &MPRConfigWidget::_horizontalSlider_zPlane_valueChanged);

	connect(ui->checkBox_xPlane, &QCheckBox::stateChanged, this, &MPRConfigWidget::_checkBox_xPlane_stateChanged);
	connect(ui->checkBox_yPlane, &QCheckBox::stateChanged, this, &MPRConfigWidget::_checkBox_yPlane_stateChanged);
	connect(ui->checkBox_zPlane, &QCheckBox::stateChanged, this, &MPRConfigWidget::_checkBox_zPlane_stateChanged);

	connect(ui->checkBox_cuttingPlanes, &QCheckBox::stateChanged, this, &MPRConfigWidget::_checkBox_cuttingPlanes_stateChanged);
}

MPRConfigWidget::~MPRConfigWidget() { }

Float MPRConfigWidget::getXPlanePosition() const {
	return ui->checkBox_xPlane->isChecked() ? _normalizedSliderValue(ui->horizontalSlider_xPlane) : Float(-1.0f);
}
void MPRConfigWidget::setXPlanePosition(Float value) {
	_setSliderPosition(ui->horizontalSlider_xPlane, ui->checkBox_xPlane, value);
}
void MPRConfigWidget::_horizontalSlider_xPlane_valueChanged(int)  const {
	_mpr.setXPlanePosition(getXPlanePosition());
}

Float MPRConfigWidget::getYPlanePosition() const {
	return ui->checkBox_yPlane->isChecked() ? _normalizedSliderValue(ui->horizontalSlider_yPlane) : Float(-1.0f);
}
void MPRConfigWidget::setYPlanePosition(Float value) {
	_setSliderPosition(ui->horizontalSlider_yPlane, ui->checkBox_yPlane, value);
}
void MPRConfigWidget::_horizontalSlider_yPlane_valueChanged(int) const {
	_mpr.setYPlanePosition(getYPlanePosition());
}

Float MPRConfigWidget::getZPlanePosition() const {
	return ui->checkBox_zPlane->isChecked() ? _normalizedSliderValue(ui->horizontalSlider_zPlane) : Float(-1.0f);
}
void MPRConfigWidget::setZPlanePosition(Float value) {
	_setSliderPosition(ui->horizontalSlider_zPlane, ui->checkBox_zPlane, value);
}
void MPRConfigWidget::_horizontalSlider_zPlane_valueChanged(int) const {
	_mpr.setZPlanePosition(getZPlanePosition());
}

bool MPRConfigWidget::getCuttingPlanesEnabled() const {
	return ui->checkBox_cuttingPlanes->isChecked();
}
void MPRConfigWidget::setCuttingPlanesEnabled(bool enabled) {
	ui->checkBox_cuttingPlanes->setChecked(enabled);
}
void MPRConfigWidget::_checkBox_cuttingPlanes_stateChanged(int) const {
	_mpr.setCuttingPlanesEnabled(getCuttingPlanesEnabled());
}

void MPRConfigWidget::_setSliderPosition(QSlider* slider, QCheckBox* sliderCheckBox, Float value) const {
	if (value >= 0.0f) {
		slider->setEnabled(true);
		sliderCheckBox->setChecked(true);
	} else {
		slider->setEnabled(false);
		sliderCheckBox->setChecked(false);
		value = Float(0.5f);
	}

	const auto minValue = slider->minimum();
	const auto maxValue = slider->maximum();

	slider->setSliderPosition(int((maxValue - minValue) * value));
}

Float MPRConfigWidget::_normalizedSliderValue(QSlider* slider) const {
	const auto value = slider->value();
	const auto minValue = slider->minimum();
	const auto maxValue = slider->maximum();
	return (value - minValue) / Float(maxValue - minValue);
}

void MPRConfigWidget::_checkBox_xPlane_stateChanged(int) const {
	ui->horizontalSlider_xPlane->setEnabled(ui->checkBox_xPlane->isChecked());
	_mpr.setXPlanePosition(getXPlanePosition());
}
void MPRConfigWidget::_checkBox_yPlane_stateChanged(int) const {
	ui->horizontalSlider_yPlane->setEnabled(ui->checkBox_yPlane->isChecked());
	_mpr.setYPlanePosition(getYPlanePosition());
}
void MPRConfigWidget::_checkBox_zPlane_stateChanged(int) const {
	ui->horizontalSlider_zPlane->setEnabled(ui->checkBox_zPlane->isChecked());
	_mpr.setZPlanePosition(getZPlanePosition());
}

}