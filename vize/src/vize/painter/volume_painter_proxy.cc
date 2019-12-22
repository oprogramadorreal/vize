#include "vize/painter/volume_painter_proxy.hpp"

#include "vize/model/transfer_function/transfer_function.hpp"

#include <lucca/painter/painter.hpp>
#include <lucca/view/view.hpp>

namespace vize {

VolumePainterProxy::VolumePainterProxy(lucca::View& view)
	: _view(view)
{ }

VolumePainterProxy::~VolumePainterProxy() = default;

template <class PainterType>
PainterType* VolumePainterProxy::_getActivePainter() const {
	return dynamic_cast<PainterType*>(_view.getPainter());
}

template <class PainterType, class T>
T VolumePainterProxy::_getFromActivePainter(std::function<T(const PainterType&)> getter) const {
	if (const auto painter = _getActivePainter<PainterType>()) {
		return getter(*painter);
	}

	return T();
}

template <class PainterType>
void VolumePainterProxy::_doForActivePainter(std::function<void(PainterType&)> action) const {
	if (const auto painter = _getActivePainter<PainterType>()) {
		action(*painter);
	}

	_view.postRedisplay();
}

void VolumePainterProxy::setXPlanePosition(Float position) {
	_doForActivePainter<VolumeMPR>(
		[position](VolumeMPR& mpr) { mpr.setXPlanePosition(position); }
	);
}
void VolumePainterProxy::setYPlanePosition(Float position) {
	_doForActivePainter<VolumeMPR>(
		[position](VolumeMPR& mpr) { mpr.setYPlanePosition(position); }
	);
}
void VolumePainterProxy::setZPlanePosition(Float position) {
	_doForActivePainter<VolumeMPR>(
		[position](VolumeMPR& mpr) { mpr.setZPlanePosition(position); }
	);
}

Float VolumePainterProxy::getXPlanePosition() const {
	return _getFromActivePainter<VolumeMPR, Float>(
		[](const VolumeMPR& mpr){ return mpr.getXPlanePosition(); }
	);
}
Float VolumePainterProxy::getYPlanePosition() const {
	return _getFromActivePainter<VolumeMPR, Float>(
		[](const VolumeMPR& mpr){ return mpr.getYPlanePosition(); }
	);
}
Float VolumePainterProxy::getZPlanePosition() const {
	return _getFromActivePainter<VolumeMPR, Float>(
		[](const VolumeMPR& mpr){ return mpr.getZPlanePosition(); }
	);
}

void VolumePainterProxy::setCuttingPlanesEnabled(bool enabled) {
	_doForActivePainter<VolumeMPR>(
		[enabled](VolumeMPR& mpr) { mpr.setCuttingPlanesEnabled(enabled); }
	);
}
bool VolumePainterProxy::getCuttingPlanesEnabled() const {
	return _getFromActivePainter<VolumeMPR, bool>(
		[](const VolumeMPR& mpr){ return mpr.getCuttingPlanesEnabled(); }
	);
}

void VolumePainterProxy::setTransferFunction(const TransferFunction& function) {
	_doForActivePainter<VolumeRaycaster>(
		[&function](VolumeRaycaster& raycaster) { raycaster.setTransferFunction(function); }
	);
}
TransferFunction VolumePainterProxy::getTransferFunction() const {
	return _getFromActivePainter<VolumeRaycaster, TransferFunction>(
		[](const VolumeRaycaster& raycaster){ return raycaster.getTransferFunction(); }
	);
}

void VolumePainterProxy::setLightingEnabled(bool enabled) {
	_doForActivePainter<VolumeRaycaster>(
		[enabled](VolumeRaycaster& raycaster) { raycaster.setLightingEnabled(enabled); }
	);
}
bool VolumePainterProxy::getLightingEnabled() const {
	return _getFromActivePainter<VolumeRaycaster, bool>(
		[](const VolumeRaycaster& raycaster){ return raycaster.getLightingEnabled(); }
	);
}

void VolumePainterProxy::setSampleStep(Float step) {
	_doForActivePainter<VolumeRaycaster>(
		[step](VolumeRaycaster& raycaster) { raycaster.setSampleStep(step); }
	);
}
Float VolumePainterProxy::getSampleStep() const {
	return _getFromActivePainter<VolumeRaycaster, Float>(
		[](const VolumeRaycaster& raycaster){ return raycaster.getSampleStep(); }
	);
}

glm::mat4 VolumePainterProxy::getModelMatrix() const {
	return _getFromActivePainter<VolumeRaycaster, glm::mat4>(
		[](const VolumeRaycaster& raycaster){ return raycaster.getModelMatrix(); }
	);
}
void VolumePainterProxy::setModelMatrix(const glm::mat4& modelMatrix) {
	_doForActivePainter<VolumeRaycaster>(
		[&modelMatrix](VolumeRaycaster& raycaster) { raycaster.setModelMatrix(modelMatrix); }
	);
}

const VolumeHistogram* VolumePainterProxy::getHistogram() const {
	return _getFromActivePainter<VolumeRaycaster, const VolumeHistogram*>(
		[](const VolumeRaycaster& raycaster){ return raycaster.getHistogram(); }
	);
}

}