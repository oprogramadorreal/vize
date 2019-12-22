#include "vize/tool/save_as_document_tool.hpp"

#include "vize/document/volume_document.hpp"
#include "vize/serialization/vize_serializer.hpp"

#include <QFileDialog>

namespace vize {

SaveAsDocumentTool::SaveAsDocumentTool(VolumeDocument& document, QWidget* parent)
	: _document(document), _parent(parent)
{ }

SaveAsDocumentTool::~SaveAsDocumentTool() = default;

void SaveAsDocumentTool::_activateImpl() {
	const auto outputFileName = QFileDialog::getSaveFileName(_parent, "Save As...", "volume", "VIZE (*.vize)").toStdString();

	if (outputFileName.empty()) {
		return;
	}

	VizeSerializer serializer;

	if (!serializer.save(_document, outputFileName)) {
		AYLA_DEBUG_MESSAGE(std::string("Serialization Status == ") + std::to_string(static_cast<int>(serializer.getStatus())));
	}
}

}