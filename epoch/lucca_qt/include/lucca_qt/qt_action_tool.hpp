#ifndef LUCCA_QT_ACTION_TOOL_HPP
#define LUCCA_QT_ACTION_TOOL_HPP

#include "lucca_qt/config.hpp"

#include <lucca/tool/tool.hpp>

#include <QAction>

namespace lucca {
	class ToolDescriptor;
}

namespace lucca_qt {

/**
 * A lucca::Tool bound to a QAction.
 *
 * @remarks The tool is activated when the action is triggered.
 *
 * @author O Programador
 */
class LUCCA_QT_API QtActionTool final : public QAction, public lucca::Tool {
public:
	QtActionTool(QObject* parent, std::unique_ptr<Tool> tool);
	virtual ~QtActionTool();

	void setToolDescriptor(std::unique_ptr<lucca::ToolDescriptor> descriptor);

	std::string getToolCursor() const;
	
	Tool* getActualTool() const;

public: // lucca::Tool interface
	virtual void activateTool() final override;
	virtual void deactivateTool() final override;
	virtual bool isToolActive() const final override;
	virtual void setToolGroup(lucca::ToolGroup* toolGroup) final override;
	virtual boost::signals2::connection connectToActivatedSignal(ActivatedSignalListener listener) final override;

private:
	void _handleTriggered(bool checked);
	
private:
	const std::unique_ptr<Tool> _tool;
	std::unique_ptr<lucca::ToolDescriptor> _descriptor;
};

}

#endif // LUCCA_QT_ACTION_TOOL_HPP