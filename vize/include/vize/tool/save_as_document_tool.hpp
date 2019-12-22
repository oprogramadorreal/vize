#ifndef VIZE_SAVE_AS_DOCUMENT_TOOL_HPP
#define VIZE_SAVE_AS_DOCUMENT_TOOL_HPP

#include "vize/config.hpp"

#include <lucca/tool/tool_impl.hpp>

class QWidget;

namespace vize {

class VolumeDocument;

/**
 * @author O Programador
 */
class SaveAsDocumentTool final : public lucca::ToolImpl {
public:
	SaveAsDocumentTool(VolumeDocument& document, QWidget* parent);
	virtual ~SaveAsDocumentTool();

private:
	virtual void _activateImpl() final override;

private:
	VolumeDocument& _document;
	QWidget* _parent = nullptr;
};

}

#endif // VIZE_SAVE_AS_DOCUMENT_TOOL_HPP