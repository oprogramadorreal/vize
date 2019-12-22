#include "vize/tool/open_document_tool.hpp"

#include "vize/document/volume_document.hpp"
#include "vize/serialization/vize_serializer.hpp"

#include <QFileDialog>

namespace vize {

OpenDocumentTool::OpenDocumentTool(VolumeDocument& document, QWidget* parent)
	: _document(document), _parent(parent)
{ }

OpenDocumentTool::~OpenDocumentTool() = default;

namespace {

void _removeSubViews(lucca::MultiViewDocument& document) {
	document.removeAllSubViews();
}

}

void OpenDocumentTool::_activateImpl() {
	const auto fileName = QFileDialog::getOpenFileName(_parent, "Open Vize document", "", "VIZE (*.vize);;XML (*.xml);; All files (*.*)");

	if (!fileName.isEmpty()) {
		open(fileName.toStdString());
	}
}

void OpenDocumentTool::open(const std::string& fileName) const {
	_removeSubViews(_document);

	VizeSerializer serializer;

	if (!serializer.load(_document, fileName)) {
		AYLA_DEBUG_MESSAGE(std::string("Serialization Status == ") + std::to_string(static_cast<int>(serializer.getStatus())));
	}
}

}