#include "vize/application/import_volume_launcher.hpp"

#include "vize/model/volume.hpp"
#include "vize/document/volume_document.hpp"
#include "vize/document/sub_view.hpp"
#include "vize/document/sub_view_state.hpp"
#include "vize/painter/volume_painter.hpp"
#include "vize/factory/impl/tiff_volume_factory.hpp"
#include "vize/factory/impl/dicom_volume_factory.hpp"
#include "vize/factory/volume_painter_factory.hpp"
#include "vize/factory/sub_view_factory.hpp"

#include <lucca/painter/painter.hpp>
#include <lucca_qt/qt_opengl_view.hpp>

#include <ayla/directory.hpp>

#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace vize {

ImportVolumeLauncher::ImportVolumeLauncher(
	VolumeDocument& document,
	std::unique_ptr<VolumeFactory> volumeFactory,
	std::unique_ptr<VolumePainterFactory> volumePainterFactory,
	std::unique_ptr<SubViewFactory> subViewFactory
)
	: _documentFactory(document, std::move(volumeFactory)), _volumePainterFactory(std::move(volumePainterFactory)), _subViewFactory(std::move(subViewFactory))
{
	_documentFactoryConnection = _documentFactory.connectToDocumentBuiltSignal(std::bind(&ImportVolumeLauncher::_documentBuilt, this, std::placeholders::_1, std::placeholders::_2));
}

ImportVolumeLauncher::~ImportVolumeLauncher() = default;

void ImportVolumeLauncher::run() {
	const auto volumeDirectory = _getVolumeDirectory();

	if (!volumeDirectory.empty()) {
		_documentFactory.clearVolumeImages();
		_documentFactory.addVolumeImagesDirectory(volumeDirectory);
		_documentFactory.run();
	} else {
		AYLA_DEBUG_MESSAGE("Unable to find volumes directory!");
	}
}

bool ImportVolumeLauncher::isRunning() const {
	return _documentFactory.isRunning();
}

void ImportVolumeLauncher::showProgressDialog(const std::string& progressDialogTitle) {
	_documentFactory.showProgressDialog(progressDialogTitle);
}

void ImportVolumeLauncher::_documentBuilt(VolumeDocument& document, Volume& volume) {
	_documentBuiltImpl(document, volume);

	if (auto volumePainter = _volumePainterFactory->create(volume)) {
		auto widgetView = std::make_unique<lucca_qt::QtOpenGLView>(std::move(volumePainter));

		if (auto subView = _subViewFactory->create(std::move(widgetView))) {
			subView->setDefaultCameraPosition();

			if (const auto garmentPainter = dynamic_cast<VolumePainter*>(subView->getPainter())) {
				subView->setState(SubViewState::fromPainter(*garmentPainter));
			}

			document.addView(std::move(subView));

		} else {
			AYLA_DEBUG_MESSAGE("Unable to create document view.");
		}
	} else {
		AYLA_DEBUG_MESSAGE("Unable to create volume painter.");
	}
}

/////////////////////////////////////////////////////

ImportCTHEAD::ImportCTHEAD(VolumeDocument& document, std::unique_ptr<VolumePainterFactory> volumePainterFactory, std::unique_ptr<SubViewFactory> subViewFactory)
	: ImportVolumeLauncher(document, std::make_unique<TiffVolumeFactory>(), std::move(volumePainterFactory), std::move(subViewFactory))
{ }

std::string ImportCTHEAD::_getVolumeDirectory() const {
	return ayla::findDirectory("vize/volumes/external/cthead-8bit/");
}

void ImportCTHEAD::_documentBuiltImpl(const VolumeDocument&, Volume& volume) {
	const auto volumeSizeInVoxels = glm::vec3(volume.getWidth(), volume.getHeight(), volume.getDepth());
	const auto volumeSizeNormalized = volumeSizeInVoxels / glm::vec3(std::max(std::max(volumeSizeInVoxels.x, volumeSizeInVoxels.y), volumeSizeInVoxels.z));
	const auto volumeScaleAdjustment = 5.0f * glm::vec3(0.95f, 1.0f, 1.8878f); // from http://lgdv.cs.fau.de/External/vollib/
	const auto volumeSize = volumeSizeNormalized * volumeScaleAdjustment;
	//volumeSpacing = volumeSize / volumeSizeInVoxels; // TODO: move this to Volume?

	auto modelMatrix = glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(180.0f), glm::vec3(0, 0, 1));

	volume.setModelMatrix(glm::scale(modelMatrix, volumeSize));
}

/////////////////////////////////////////////////////

ImportMANIX::ImportMANIX(VolumeDocument& document, std::unique_ptr<VolumePainterFactory> volumePainterFactory, std::unique_ptr<SubViewFactory> subViewFactory)
	: ImportVolumeLauncher(document, std::make_unique<DicomVolumeFactory>(), std::move(volumePainterFactory), std::move(subViewFactory))
{ }

std::string ImportMANIX::_getVolumeDirectory() const {
	return ayla::findDirectory("vize/volumes/external/MANIX/CER-CT/ANGIO CT/");
}

void ImportMANIX::_documentBuiltImpl(const VolumeDocument& document, Volume& volume) {
	const auto volumeScale = 5.0f;
	const auto volumeProportions = glm::vec3(1.0f, 1.0f, 0.6f);
	const auto volumeSize = volumeScale * volumeProportions;
	const auto modelMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0)) * volume.getModelMatrix();
	volume.setModelMatrix(glm::scale(modelMatrix, volumeSize));
}

}