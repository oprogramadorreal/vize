#include "vize/tool/import_tiff_tool.hpp"

#include "vize/factory/impl/tiff_volume_factory.hpp"

#include <QFileDialog>

namespace vize {

ImportTiffTool::ImportTiffTool(VolumeDocument& document, QWidget* parent)
	: ImportVolumeTool(document, std::make_unique<TiffVolumeFactory>()), _parent(parent)
{ }

ImportTiffTool::~ImportTiffTool() = default;

void ImportTiffTool::_activateImpl() {
	const auto dialogTitle = std::string("Import TIFF");
	const auto selectedFiles = QFileDialog::getOpenFileNames(_parent, QString::fromStdString(dialogTitle), QString(""), QString("TIFF Images (*.tif *.tiff)"));
	_buildDocument(selectedFiles, dialogTitle);
}

}