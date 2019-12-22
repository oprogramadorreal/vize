#include "vize/document/volume_document.hpp"

#include "vize/model/volume.hpp"
#include "vize/painter/volume_painter.hpp"

#include <lucca/view/view_composite.hpp>

namespace vize {

VolumeDocument::VolumeDocument(const std::string& documentName, std::unique_ptr<lucca::ViewComposite> viewComposite)
	: SimpleMultiViewDocument(std::move(viewComposite)), _documentName(documentName)
{ }

VolumeDocument::~VolumeDocument() = default;

void VolumeDocument::setVolume(const std::shared_ptr<Volume>& volume) {
	removeAllSubViews();
	_volume = volume;
}

void VolumeDocument::setVolumeTextureCache(const std::shared_ptr<GLVolumeTextureCache>& volumeTextureCache) {
	_volumeTextureCache = volumeTextureCache;
}

glm::mat4 VolumeDocument::getModelMatrix() const {
	return _volume ? _volume->getModelMatrix() : glm::mat4(1.0f);
}

void VolumeDocument::setModelMatrix(const glm::mat4& modelMatrix) {
	if (_volume) {
		_volume->setModelMatrix(modelMatrix);
	}
}

SizeType VolumeDocument::getVolumeWidth() const {
	return _volume ? _volume->getWidth() : SizeType(0);
}

SizeType VolumeDocument::getVolumeHeight() const {
	return _volume ? _volume->getHeight() : SizeType(0);
}

SizeType VolumeDocument::getVolumeDepth() const {
	return _volume ? _volume->getDepth() : SizeType(0);
}

void VolumeDocument::setDocumentName(const std::string& name) {
	_documentName = name;
}

std::string VolumeDocument::getDocumentName() const {
	return _documentName;
}

void VolumeDocument::addView(std::unique_ptr<lucca::View> view) {
	if (_volumeTextureCache) {
		if (const auto volumePainter = dynamic_cast<VolumePainter*>(view->getPainter())) {
			volumePainter->setVolumeTextureCache(_volumeTextureCache.get());
		}
	}

	_addSubView(std::move(view));
}

}