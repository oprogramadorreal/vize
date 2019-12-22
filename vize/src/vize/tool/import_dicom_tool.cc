#include "vize/tool/import_dicom_tool.hpp"

#include "vize/factory/impl/dicom_volume_factory.hpp"

#include <QFileDialog>

namespace vize {

ImportDicomTool::ImportDicomTool(VolumeDocument& document, QWidget* parent)
	: ImportVolumeTool(document, std::make_unique<DicomVolumeFactory>()), _parent(parent)
{ }

ImportDicomTool::~ImportDicomTool() = default;

void ImportDicomTool::_activateImpl() {
	const auto dialogTitle = std::string("Import DICOM");
	const auto selectedFiles = QFileDialog::getOpenFileNames(_parent, QString::fromStdString(dialogTitle), QString(""));
	_buildDocument(selectedFiles, dialogTitle);
}

}