#ifndef VIZE_MODEL_TRANSFORMER_WIDGET_HPP
#define VIZE_MODEL_TRANSFORMER_WIDGET_HPP

#include "vize/config.hpp"

#include <QWidget>

namespace Ui {
	class ModelTransformerWidget;
}

namespace vize {

class VolumeRaycaster;

/**
 * @author O Programador
 */
class ModelTransformerWidget final : public QWidget { 
public:
	explicit ModelTransformerWidget(VolumeRaycaster& volumePainter, QWidget* parent);
	virtual ~ModelTransformerWidget();

private:
	void _pushButton_rotateLeft_pressed();
	void _pushButton_rotateRight_pressed();
	void _pushButton_scale_pressed();
	void _pushButton_scaleAxis_pressed();
	void _pushButton_setIdentity_pressed();

private:
	void _rotate(Float angleRadians, const glm::vec3& axisWorld) const;
	void _scale(const glm::vec3& scaleStep) const;

	glm::vec3 _getAxisWorld() const;
	Float _getAngleRadiansStep() const;
	Float _getScaleStep() const;

private:
	VolumeRaycaster& _raycaster;
	std::unique_ptr<Ui::ModelTransformerWidget> ui;
};

}

#endif // VIZE_MODEL_TRANSFORMER_WIDGET_HPP