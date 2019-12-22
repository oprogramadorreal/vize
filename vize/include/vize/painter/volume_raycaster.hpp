#ifndef VIZE_VOLUME_RAYCASTER_HPP
#define VIZE_VOLUME_RAYCASTER_HPP

#include "vize/config.hpp"

namespace vize {

class TransferFunction;
class VolumeHistogram;

/**
 * Volume ray casting interface.
 *
 * @see https://en.wikipedia.org/wiki/Volume_ray_casting
 *
 * @author O Programador
 */
class VolumeRaycaster {
public:
	virtual ~VolumeRaycaster() = default;

	virtual void setTransferFunction(const TransferFunction& function) = 0;
	virtual TransferFunction getTransferFunction() const = 0;

	virtual void setLightingEnabled(bool enabled) = 0;
	virtual bool getLightingEnabled() const = 0;

	virtual void setSampleStep(Float step) = 0;
	virtual Float getSampleStep() const = 0;

	virtual void setModelMatrix(const glm::mat4& modelMatrix) = 0;
	virtual glm::mat4 getModelMatrix() const = 0;

	virtual const VolumeHistogram* getHistogram() const = 0;
};

}

#endif // VIZE_VOLUME_RAYCASTER_HPP