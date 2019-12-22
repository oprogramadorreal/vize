#ifndef VIZE_SUB_VIEW_STATE_HPP
#define VIZE_SUB_VIEW_STATE_HPP

#include "vize/config.hpp"

#include "vize/model/transfer_function/transfer_function.hpp"

namespace vize {

class VolumePainter;

/**
 * @author O Programador
 */
class SubViewState final {
public:
	SubViewState();
	virtual ~SubViewState();

public:
	static SubViewState fromPainter(const VolumePainter& painter);

public:
	bool getShowModelTransformer() const;
	void setShowModelTransformer(bool show);

	void setTransferFunction(const TransferFunction& function);
	const TransferFunction& getTransferFunction() const;

	void setXPlanePosition(Float position);
	void setYPlanePosition(Float position);
	void setZPlanePosition(Float position);

	Float getXPlanePosition() const;
	Float getYPlanePosition() const;
	Float getZPlanePosition() const;

	void setCuttingPlanesEnabled(bool enabled);
	bool getCuttingPlanesEnabled() const;

	void setLightingEnabled(bool enabled);
	bool getLightingEnabled() const;

	void setSampleStep(Float step);
	Float getSampleStep() const;

private:
	TransferFunction _tFunction;
	Float _xPlane = Float(0.0f);
	Float _yPlane = Float(0.0f);
	Float _zPlane = Float(0.0f);
	bool _cuttingPlanes = false;
	bool _lighting = false;
	Float _sampleStep = 0.0f;
	bool _modelTransformer = false;
};

}

#endif // VIZE_SUB_VIEW_STATE_HPP