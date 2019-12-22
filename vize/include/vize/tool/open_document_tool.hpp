#ifndef VIZE_OPEN_DOCUMENT_TOOL_HPP
#define VIZE_OPEN_DOCUMENT_TOOL_HPP

#include "vize/config.hpp"

#include <lucca/tool/tool_impl.hpp>

class QWidget;

namespace vize {

class VolumeDocument;

/**
 * @author O Programador
 */
class OpenDocumentTool final : public lucca::ToolImpl {
public:
	OpenDocumentTool(VolumeDocument& document, QWidget* parent = nullptr);
	virtual ~OpenDocumentTool();

	void open(const std::string& fileName) const;

private:
	virtual void _activateImpl() final override;

private:
	VolumeDocument& _document;
	QWidget* _parent = nullptr;
};

}

#endif // VIZE_OPEN_DOCUMENT_TOOL_HPP