#ifndef VIZE_VOLUME_PAINTER_PROXY_HPP
#define VIZE_VOLUME_PAINTER_PROXY_HPP

#include "vize/config.hpp"

#include "vize/painter/volume_raycaster.hpp"
#include "vize/painter/volume_mpr.hpp"

#include <functional>

namespace lucca {
	class View;
}

namespace vize {

/**
 * Acts on the volume raycaster of the active view.
 *
 * @author O Programador
 */
class VolumePainterProxy final : public VolumeRaycaster, public VolumeMPR {
public:
	VolumePainterProxy(lucca::View& view);
	virtual ~VolumePainterProxy();

private: // vize::VolumeMPR interface
	virtual void setXPlanePosition(Float position) final override;
	virtual void setYPlanePosition(Float position) final override;
	virtual void setZPlanePosition(Float position) final override;
	virtual Float getXPlanePosition() const final override;
	virtual Float getYPlanePosition() const final override;
	virtual Float getZPlanePosition() const final override;
	virtual void setCuttingPlanesEnabled(bool enabled) final override;
	virtual bool getCuttingPlanesEnabled() const final override;

private: // vize::VolumeRaycaster interface
	virtual void setTransferFunction(const TransferFunction& function) final override;
	virtual TransferFunction getTransferFunction() const final override;
	virtual void setLightingEnabled(bool enabled) final override;
	virtual bool getLightingEnabled() const final override;
	virtual void setSampleStep(Float step) final override;
	virtual Float getSampleStep() const final override;
	virtual void setModelMatrix(const glm::mat4& modelMatrix) final override;
	virtual glm::mat4 getModelMatrix() const final override;
	virtual const VolumeHistogram* getHistogram() const final override;

private:
	template <class PainterType>
	PainterType* _getActivePainter() const;

	template <class PainterType, class T>
	T _getFromActivePainter(std::function<T(const PainterType&)> getter) const;

	template <class PainterType>
	void _doForActivePainter(std::function<void(PainterType&)> action) const;

private:
	lucca::View& _view;
};

}

#endif // VIZE_VOLUME_PAINTER_PROXY_HPP