#include "vize/tool/import_volume_tool.hpp"

#include "vize/factory/volume_factory.hpp"
#include "vize/factory/volume_painter_factory.hpp"
#include "vize/factory/sub_view_factory.hpp"
#include "vize/document/volume_document.hpp"
#include "vize/document/sub_view.hpp"
#include "vize/document/sub_view_state.hpp"
#include "vize/painter/volume_painter.hpp"

#include <lucca_qt/qt_opengl_view.hpp>

namespace vize {

ImportVolumeTool::ImportVolumeTool(VolumeDocument& document, std::unique_ptr<VolumeFactory> volumeFactory)
	: _document(document), _documentFactory(document, std::move(volumeFactory))
{
	_documentFactoryConnection = _documentFactory.connectToDocumentBuiltSignal(
		[this](VolumeDocument& document, Volume& volume) {
			_documentBuilt(document, volume);
		}
	);
}

ImportVolumeTool::~ImportVolumeTool() = default;

void ImportVolumeTool::_buildDocument(const QStringList& volumeFiles, const std::string& loadingDialogTitle) {
	if (!volumeFiles.empty()) {
		_documentFactory.clearVolumeImages();

		for (auto& fileName : volumeFiles) {
			_documentFactory.addVolumeImage(fileName.toStdString());
		}

		_documentFactory.run();
		_documentFactory.showProgressDialog(loadingDialogTitle);
	}
}

void ImportVolumeTool::_documentBuilt(VolumeDocument& document, Volume& volume) {
	VolumePainterFactory volumePainterFactory;
	SubViewFactory subViewFactory;

	if (auto volumePainter = volumePainterFactory.create(volume)) {
		auto widgetView = std::make_unique<lucca_qt::QtOpenGLView>(std::move(volumePainter));

		if (auto subView = subViewFactory.create(std::move(widgetView))) {
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

}