#ifndef VIZE_SAMPLE_STEP_WIDGET_HPP
#define VIZE_SAMPLE_STEP_WIDGET_HPP

#include "vize/config.hpp"

#include <QWidget>

namespace Ui {
	class SampleStepWidget;
}

namespace vize {

class VolumeRaycaster;

/**
 * @author O Programador
 */
class SampleStepWidget final : public QWidget {
public:
	explicit SampleStepWidget(VolumeRaycaster& raycaster, QWidget* parent);
	virtual ~SampleStepWidget();

public:
	Float getSampleStep() const;
	void setSampleStep(Float step);

private:
	void _pushButton_lighting_pressed(bool) const;
	void _doubleSpinBox_sampleStep_valueChanged(double value) const;

private:
	VolumeRaycaster& _raycaster;
	std::unique_ptr<Ui::SampleStepWidget> ui;
};

}

#endif // VIZE_SAMPLE_STEP_WIDGET_HPP