#include "vize/factory/async_volume_document_factory.hpp"

#include "vize/factory/volume_factory.hpp"
#include "vize/document/volume_document.hpp"

namespace vize {

AsyncVolumeDocumentFactory::AsyncVolumeDocumentFactory(VolumeDocument& document, std::unique_ptr<VolumeFactory> factory)
	: _document(document), _volumeFactory(std::move(factory))
{
	_volumeFactory.connectToVolumeBuiltSignal(std::bind(&AsyncVolumeDocumentFactory::_volumeBuilt, this, std::placeholders::_1, std::placeholders::_2));
}

AsyncVolumeDocumentFactory::~AsyncVolumeDocumentFactory() = default;

void AsyncVolumeDocumentFactory::run() {
	_volumeFactory.run();
}

bool AsyncVolumeDocumentFactory::isRunning() const {
	return _volumeFactory.isRunning();
}

void AsyncVolumeDocumentFactory::showProgressDialog(const std::string& progressDialogTitle) {
	_volumeFactory.showProgressDialog(progressDialogTitle);
}

void AsyncVolumeDocumentFactory::addVolumeImagesDirectory(const std::string& imagesDirectory) {
	_volumeFactory.addVolumeImagesDirectory(imagesDirectory);
}

void AsyncVolumeDocumentFactory::addVolumeImage(const std::string& imageFile) {
	_volumeFactory.addVolumeImage(imageFile);
}

void AsyncVolumeDocumentFactory::clearVolumeImages() {
	_volumeFactory.clearVolumeImages();
}

void AsyncVolumeDocumentFactory::_volumeBuilt(const std::shared_ptr<Volume>& volume, const std::string& volumeName) {
	if (volume) {
		_document.setDocumentName(volumeName);
		_document.setVolume(volume);
		_documentBuilt(_document, *volume);
	} else {
		AYLA_DEBUG_MESSAGE("Unable to create volume.");
	}
}

boost::signals2::connection AsyncVolumeDocumentFactory::connectToDocumentBuiltSignal(DocumentBuiltSignalListener listener) {
	return _documentBuilt.connect(listener);
}

}