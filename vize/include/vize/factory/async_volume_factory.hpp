#ifndef VIZE_ASYNC_VOLUME_FACTORY_HPP
#define VIZE_ASYNC_VOLUME_FACTORY_HPP

#include "vize/config.hpp"

#include <boost/signals2.hpp>

#include <vector>

namespace lucca_qt {
	class Async;
}

namespace vize {

class Volume;
class VolumeFactory;

/**
 * @author O Programador
 */
class AsyncVolumeFactory final {
public:
	AsyncVolumeFactory(std::unique_ptr<VolumeFactory> factory);
	virtual ~AsyncVolumeFactory();

public:
	/**
	 * Run the volume importation asynchronously.
	 */
	void run();

	bool isRunning() const;

	void showProgressDialog(const std::string& progressDialogTitle);

	void addVolumeImagesDirectory(const std::string& imagesDirectory);
	void addVolumeImage(const std::string& imageFile);
	void clearVolumeImages();

public:
	using VolumeBuiltSignal = boost::signals2::signal<void(const std::shared_ptr<Volume>& volume, const std::string& volumeName)>;
	using VolumeBuiltSignalListener = VolumeBuiltSignal::slot_function_type;

	/**
	 * Register a listener to be notified when a volume is created.
	 */
	boost::signals2::connection connectToVolumeBuiltSignal(const VolumeBuiltSignalListener& listener);

private:
	void _buildVolumeFinished();
	void _buildVolume();

private:
	const std::unique_ptr<VolumeFactory> _factory;
	std::unique_ptr<lucca_qt::Async> _async;
	std::shared_ptr<Volume> _volume;
	std::vector<std::string> _volumeImages;
	VolumeBuiltSignal _volumeBuilt;
};

}

#endif // VIZE_ASYNC_VOLUME_FACTORY_HPP