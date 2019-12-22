#include "vize/factory/async_volume_factory.hpp"

#include "vize/model/volume.hpp"
#include "vize/factory/volume_factory.hpp"

#include <lucca_qt/async.hpp>
#include <ayla/directory.hpp>

#include <boost/filesystem.hpp>

namespace vize {

AsyncVolumeFactory::AsyncVolumeFactory(std::unique_ptr<VolumeFactory> factory)
	: _factory(std::move(factory))
{
	assert(_factory);

	const auto buildVolumeTask = std::bind(&AsyncVolumeFactory::_buildVolume, this);
	_async = std::make_unique<lucca_qt::Async>(buildVolumeTask);
	_async->connectToTaskFinishedSignal(std::bind(&AsyncVolumeFactory::_buildVolumeFinished, this));
	_factory->connectAsync(*_async);
}

AsyncVolumeFactory::~AsyncVolumeFactory() = default;

void AsyncVolumeFactory::run() {
	if (!_volumeImages.empty()) {
		_async->run();
	}
}

bool AsyncVolumeFactory::isRunning() const {
	return _async && _async->isRunning();
}

void AsyncVolumeFactory::showProgressDialog(const std::string& progressDialogTitle) {
	if (_async) {
		_async->showProgressDialog(progressDialogTitle);
	}
}

void AsyncVolumeFactory::addVolumeImagesDirectory(const std::string& imagesDirectory) {
	const auto images = ayla::getFilesPathInDirectory(imagesDirectory);

	for (const auto& i : images) {
		addVolumeImage(i);
	}
}

void AsyncVolumeFactory::addVolumeImage(const std::string& imageFile) {
	_volumeImages.push_back(imageFile);
}

void AsyncVolumeFactory::clearVolumeImages() {
	_volumeImages.clear();
}

boost::signals2::connection AsyncVolumeFactory::connectToVolumeBuiltSignal(const VolumeBuiltSignalListener& listener) {
	return _volumeBuilt.connect(listener);
}

void AsyncVolumeFactory::_buildVolumeFinished() {
	if (!_volume) {
		AYLA_DEBUG_MESSAGE("Unable to load volume!");
		return;
	}

	auto volumeName = std::string();

	if (!_volumeImages.empty()) {
		volumeName = boost::filesystem::path(_volumeImages.front()).parent_path().stem().string();
	}

	_volumeBuilt(_volume, volumeName);

	_volume.reset();
}

void AsyncVolumeFactory::_buildVolume() { // this will run asynchronously (in another thread)
	_volume = std::shared_ptr<Volume>(_factory->createVolume(_volumeImages).release());
}

}