#include "vize/ui/model_transformer_widget.hpp"
#include "ui_model_transformer_widget.h"

#include "vize/painter/volume_raycaster.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace vize {

ModelTransformerWidget::ModelTransformerWidget(VolumeRaycaster& raycaster, QWidget* parent)
	: QWidget(parent), _raycaster(raycaster), ui(std::make_unique<Ui::ModelTransformerWidget>())
{
	ui->setupUi(this);

	connect(ui->pushButton_rotateLeft, &QPushButton::pressed, this, &ModelTransformerWidget::_pushButton_rotateLeft_pressed);
	connect(ui->pushButton_rotateRight, &QPushButton::pressed, this, &ModelTransformerWidget::_pushButton_rotateRight_pressed);
	connect(ui->pushButton_scale, &QPushButton::pressed, this, &ModelTransformerWidget::_pushButton_scale_pressed);
	connect(ui->pushButton_scaleAxis, &QPushButton::pressed, this, &ModelTransformerWidget::_pushButton_scaleAxis_pressed);
	connect(ui->pushButton_setIdentity, &QPushButton::pressed, this, &ModelTransformerWidget::_pushButton_setIdentity_pressed);
}

ModelTransformerWidget::~ModelTransformerWidget() = default;

void ModelTransformerWidget::_rotate(Float angleRadians, const glm::vec3& axisWorld) const {
	const auto rotationMatrix = glm::rotate(glm::mat4(1.0f), angleRadians, axisWorld);
	const auto newModelMatrix = rotationMatrix * _raycaster.getModelMatrix();
	_raycaster.setModelMatrix(newModelMatrix);
}

void ModelTransformerWidget::_scale(const glm::vec3& scaleStep) const {
	const auto scaleMatrix = glm::scale(glm::mat4(1.0f), scaleStep);
	const auto newModelMatrix = scaleMatrix * _raycaster.getModelMatrix();
	_raycaster.setModelMatrix(newModelMatrix);
}

void ModelTransformerWidget::_pushButton_rotateLeft_pressed() {
	_rotate(-_getAngleRadiansStep(), _getAxisWorld());
}

void ModelTransformerWidget::_pushButton_rotateRight_pressed() { 
	_rotate(_getAngleRadiansStep(), _getAxisWorld());
}

void ModelTransformerWidget::_pushButton_scale_pressed() {
	_scale(glm::vec3(1.0f) * _getScaleStep());
}

void ModelTransformerWidget::_pushButton_scaleAxis_pressed() {
	if (ui->radioButton_xAxis->isChecked()) {
		_scale(glm::vec3(_getScaleStep(), 1.0f, 1.0f));
	} else if (ui->radioButton_yAxis->isChecked()) {
		_scale(glm::vec3(1.0f, _getScaleStep(), 1.0f));
	} else {
		_scale(glm::vec3(1.0f, 1.0f, _getScaleStep()));
	}
}

void ModelTransformerWidget::_pushButton_setIdentity_pressed() {
	_raycaster.setModelMatrix(glm::mat4(1.0f));
}

glm::vec3 ModelTransformerWidget::_getAxisWorld() const {
	glm::vec3 axis;

	if (ui->radioButton_xAxis->isChecked()) {
		axis = glm::vec3(1.0f, 0.0f, 0.0f);
	} else if (ui->radioButton_yAxis->isChecked()) {
		axis = glm::vec3(0.0f, 1.0f, 0.0f);
	} else {
		axis = glm::vec3(0.0f, 0.0f, 1.0f);
	}

	return glm::normalize(axis);
}

Float ModelTransformerWidget::_getAngleRadiansStep() const {
	return Float(glm::radians(ui->doubleSpinBox_angle->value()));
}

Float ModelTransformerWidget::_getScaleStep() const {
	return Float(ui->doubleSpinBox_scaleStep->value());
}

}