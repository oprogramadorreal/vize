#ifndef VIZE_MPR_CONFIG_WIDGET_HPP
#define VIZE_MPR_CONFIG_WIDGET_HPP

#include "vize/config.hpp"

#include <QWidget>

class QSlider;
class QCheckBox;

namespace Ui {
	class MPRConfigWidget;
}

namespace vize {

class VolumeMPR;

/**
 * User interface (UI) for setting MPR properties.
 *
 * @author O Programador
 */
class MPRConfigWidget final : public QWidget {
public:
	explicit MPRConfigWidget(VolumeMPR& volumePainter, QWidget* parent);
	virtual ~MPRConfigWidget();

public:
	Float getXPlanePosition() const;
	void setXPlanePosition(Float value);

	Float getYPlanePosition() const;
	void setYPlanePosition(Float value);

	Float getZPlanePosition() const;
	void setZPlanePosition(Float value);

	bool getCuttingPlanesEnabled() const;
	void setCuttingPlanesEnabled(bool enabled);

private:
	void _horizontalSlider_xPlane_valueChanged(int value) const;
	void _horizontalSlider_yPlane_valueChanged(int value) const;
	void _horizontalSlider_zPlane_valueChanged(int value) const;

	void _checkBox_xPlane_stateChanged(int state) const;
	void _checkBox_yPlane_stateChanged(int state) const;
	void _checkBox_zPlane_stateChanged(int state) const;

	void _checkBox_cuttingPlanes_stateChanged(int state) const;

private:
	Float _normalizedSliderValue(QSlider* slider) const;
	void _setSliderPosition(QSlider* slider, QCheckBox* sliderCheckBox, Float value) const;

private:
	VolumeMPR& _mpr;
	std::unique_ptr<Ui::MPRConfigWidget> ui;
};

}

#endif // VIZE_MPR_CONFIG_WIDGET_HPP