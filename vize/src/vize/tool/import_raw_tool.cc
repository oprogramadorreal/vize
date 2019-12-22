#include "vize/tool/import_raw_tool.hpp"

#include "vize/factory/impl/raw_volume_factory.hpp"

#include <QFileDialog>

namespace vize {

ImportRawTool::ImportRawTool(VolumeDocument& document, QWidget* parent)
	: ImportVolumeTool(document, std::make_unique<RawVolumeFactory>()), _parent(parent)
{ }

ImportRawTool::~ImportRawTool() = default;

void ImportRawTool::_activateImpl() {
	const auto dialogTitle = std::string("Import RAW");
	const auto selectedFiles = QFileDialog::getOpenFileNames(_parent, QString::fromStdString(dialogTitle), QString(""));
	_buildDocument(selectedFiles, dialogTitle);
}

}