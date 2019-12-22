#ifndef VIZE_VOLUME_FACTORY_HPP
#define VIZE_VOLUME_FACTORY_HPP

#include "vize/config.hpp"

#include <vector>
#include <string>

#include <lucca_qt/async.hpp>

namespace ayla {
	class Image;
}

namespace vize {

class Volume;

/**
 * Knows how to create a vize::Volume from a sequence of images.
 *
 * @see vize::Volume
 *
 * @see vize::TiffVolumeFactory
 * @see vize::DicomVolumeFactory
 * @see vize::RawVolumeFactory
 *
 * @author O Programador
 */
class VolumeFactory {
public:
	VolumeFactory();
	virtual ~VolumeFactory();

public:
	std::unique_ptr<Volume> createVolume(const std::vector<std::string>& images);
	std::unique_ptr<Volume> createVolumeFromDirectory(const std::string& imagesDirectory);

	void connectAsync(lucca_qt::Async& async);

protected:
	SizeType _getExpectedImagePixelSizeInBytes() const;

	bool _loadVolumeRawData(const std::vector<std::string>& allImages, std::vector<std::uint8_t>& volumeData, SizeType& volumeWidth, SizeType& volumeHeight, SizeType& volumeDepth);
	std::unique_ptr<Volume> _loadVolumeAsRawData(const std::vector<std::string>& allImages);

private:
	std::unique_ptr<ayla::Image> _createImage(const std::string& imageFile);

private:
	virtual std::unique_ptr<ayla::Image> _createImageImpl(const std::string& imageFile) const = 0;
	virtual std::unique_ptr<Volume> _createVolumeImpl(const std::vector<std::string>& images) = 0;

private:
	std::atomic<bool> _asyncCanceled = false;
	lucca_qt::Async::TaskProgressSignal _progressSignal;
	std::vector<boost::signals2::scoped_connection> _asyncConnections;
	boost::optional<SizeType> _expectedImagePixelSizeInBytes;
};

}

#endif // VIZE_VOLUME_FACTORY_HPP