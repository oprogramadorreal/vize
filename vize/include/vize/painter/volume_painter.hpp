#ifndef VIZE_VOLUME_PAINTER_HPP
#define VIZE_VOLUME_PAINTER_HPP

#include "vize/config.hpp"

#include "vize/painter/volume_raycaster.hpp"
#include "vize/painter/volume_mpr.hpp"
#include "vize/model/transfer_function/transfer_function.hpp"
#include "vize/serialization/volume_painter_serializer.hpp"

#include <lucca/painter/painter_impl.hpp>

namespace ayla {
	class AxisAlignedBox;
}

namespace vize {

class Volume;
class GLVolumeTextureCache;

/**
 * Knows how to draw a volume.
 *
 * @author O Programador
 */
class VolumePainter final : public lucca::PainterImpl, public VolumeRaycaster, public VolumeMPR {
public:
	VolumePainter();
	VolumePainter(Volume* volume);

	virtual ~VolumePainter();

	void setVolumeTextureCache(GLVolumeTextureCache* volumeTextureCache);

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

private: // lucca::PainterImpl interface
	virtual void _initializeImpl(lucca::RenderTarget* target) final override;
	virtual void _disposeImpl() final override;
	virtual void _paintImpl(const lucca::Camera& camera) final override;
	virtual void _resizeImpl(SizeType width, SizeType height) final override;
	virtual ayla::AxisAlignedBox _getAABBImpl() const final override;
	virtual std::unique_ptr<Painter> _cloneImpl() const final override;

private:
	glm::vec3 _toNormalizedModelSpace(const glm::vec3& pointWorld) const;
	glm::vec3 _calculateVolumeSpacing() const;

private:
	Volume* _volume = nullptr;
	GLVolumeTextureCache* _volumeTextureCache = nullptr;

	TransferFunction _transferFunction;
	Float _xPlanePosition = Float(-1.0f);
	Float _yPlanePosition = Float(-1.0f);
	Float _zPlanePosition = Float(-1.0f);
	bool _cuttingPlanesEnabled = false;
	bool _lightingEnabled = false;
	Float _samplesStep = 0.001f;

	struct GLData;
	std::unique_ptr<GLData> _gl;

	template<class Archive> friend void boost::serialization::serialize(Archive&, vize::VolumePainter&, const unsigned int);
	template<class Archive> friend void boost::serialization::load_construct_data(Archive&, vize::VolumePainter*, const unsigned int);
};

}

#endif // VIZE_VOLUME_PAINTER_HPP