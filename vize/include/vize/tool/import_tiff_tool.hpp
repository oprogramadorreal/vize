#ifndef VIZE_IMPORT_TIFF_TOOL_HPP
#define VIZE_IMPORT_TIFF_TOOL_HPP

#include "vize/config.hpp"

#include "vize/tool/import_volume_tool.hpp"

class QWidget;

namespace vize {

/**
 * @author O Programador
 */
class ImportTiffTool final : public ImportVolumeTool {
public:
	ImportTiffTool(VolumeDocument& document, QWidget* parent);
	virtual ~ImportTiffTool();

private:
	virtual void _activateImpl() final override;

private:
	QWidget* _parent = nullptr;
};

}

#endif // VIZE_IMPORT_TIFF_TOOL_HPP