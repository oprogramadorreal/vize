#include "vize/document/sub_view_state.hpp"

#include "vize/painter/volume_painter.hpp"

namespace vize {

SubViewState::SubViewState() = default;
SubViewState::~SubViewState() = default;

namespace {

void _readMPR(SubViewState& state, const VolumeMPR& mpr) {
	state.setXPlanePosition(mpr.getXPlanePosition());
	state.setYPlanePosition(mpr.getYPlanePosition());
	state.setZPlanePosition(mpr.getZPlanePosition());
	state.setCuttingPlanesEnabled(mpr.getCuttingPlanesEnabled());
}

void _readRaycaster(SubViewState& state, const VolumeRaycaster& raycaster) {
	state.setTransferFunction(raycaster.getTransferFunction());
	state.setLightingEnabled(raycaster.getLightingEnabled());
	state.setSampleStep(raycaster.getSampleStep());
}

}

SubViewState SubViewState::fromPainter(const VolumePainter& painter) {
	SubViewState state;
	_readMPR(state, painter);
	_readRaycaster(state, painter);
	return state;
}

bool SubViewState::getShowModelTransformer() const {
	return _modelTransformer;
}

void SubViewState::setShowModelTransformer(bool show) {
	_modelTransformer = show;
}

void SubViewState::setTransferFunction(const TransferFunction& function) {
	_tFunction = function;
}

const TransferFunction& SubViewState::getTransferFunction() const  {
	return _tFunction;
}

void SubViewState::setXPlanePosition(Float position) {
	_xPlane = position;
}

void SubViewState::setYPlanePosition(Float position) {
	_yPlane = position;
}

void SubViewState::setZPlanePosition(Float position) {
	_zPlane = position;
}

Float SubViewState::getXPlanePosition() const {
	return _xPlane;
}

Float SubViewState::getYPlanePosition() const {
	return _yPlane;
}

Float SubViewState::getZPlanePosition() const {
	return _zPlane;
}

void SubViewState::setCuttingPlanesEnabled(bool enabled) {
	_cuttingPlanes = enabled;
}

bool SubViewState::getCuttingPlanesEnabled() const {
	return _cuttingPlanes;
}

void SubViewState::setLightingEnabled(bool enabled) {
	_lighting = enabled;
}

bool SubViewState::getLightingEnabled() const {
	return _lighting;
}

void SubViewState::setSampleStep(Float step) {
	_sampleStep = step;
}

Float SubViewState::getSampleStep() const {
	return _sampleStep;
}

}