#ifndef VIZE_RAW_VOLUME_FACTORY_HPP
#define VIZE_RAW_VOLUME_FACTORY_HPP

#include "vize/config.hpp"

#include "vize/factory/volume_factory.hpp"

namespace vize {

/**
 * Knows how to create a vize::Volume from a sequence of RAW images.
 *
 * @author O Programador
 */
class RawVolumeFactory final : public VolumeFactory {
private:
	virtual std::unique_ptr<ayla::Image> _createImageImpl(const std::string& imageFile) const final override;
	virtual std::unique_ptr<Volume> _createVolumeImpl(const std::vector<std::string>& images) final override;
};

}

#endif // VIZE_RAW_VOLUME_FACTORY_HPP