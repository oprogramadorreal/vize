#ifndef VIZE_IMPORT_RAW_TOOL_HPP
#define VIZE_IMPORT_RAW_TOOL_HPP

#include "vize/config.hpp"

#include "vize/tool/import_volume_tool.hpp"

class QWidget;

namespace vize {

/**
 * @author O Programador
 */
class ImportRawTool final : public ImportVolumeTool {
public:
	ImportRawTool(VolumeDocument& document, QWidget* parent);
	virtual ~ImportRawTool();

private:
	virtual void _activateImpl() final override;

private:
	QWidget* _parent = nullptr;
};

}

#endif // VIZE_IMPORT_RAW_TOOL_HPP