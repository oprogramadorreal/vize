#ifndef VIZE_VOLUME_MPR_HPP
#define VIZE_VOLUME_MPR_HPP

#include "vize/config.hpp"

namespace vize {

/**
 * Volume multiplanar reconstruction (MPR) interface.
 *
 * @see https://en.wikipedia.org/wiki/CT_scan#Multiplanar_reconstruction
 *
 * @author O Programador
 */
class VolumeMPR {
public:
	virtual ~VolumeMPR() = default;

	virtual void setXPlanePosition(Float position) = 0;
	virtual void setYPlanePosition(Float position) = 0;
	virtual void setZPlanePosition(Float position) = 0;

	virtual Float getXPlanePosition() const = 0;
	virtual Float getYPlanePosition() const = 0;
	virtual Float getZPlanePosition() const = 0;

	virtual void setCuttingPlanesEnabled(bool enabled) = 0;
	virtual bool getCuttingPlanesEnabled() const = 0;
};

}

#endif // VIZE_VOLUME_MPR_HPP