#ifndef LUCCA_QT_ACTION_TOOL_GROUP_HPP
#define LUCCA_QT_ACTION_TOOL_GROUP_HPP

#include "lucca_qt/config.hpp"

#include <lucca/tool/tool_group.hpp>

#include <boost/signals2.hpp>

class QObject;
class QAction;
class QActionGroup;

namespace lucca_qt {

class QtActionTool;

/**
 * @author O Programador
 */
class LUCCA_QT_API QtActionToolGroup : public lucca::ToolGroup {
public:
	QtActionToolGroup(QObject* parent);
	virtual ~QtActionToolGroup();

public:
	QtActionTool* getActiveActionTool() const;

public:
	using ActionToolActivatedSignal = boost::signals2::signal<void(QtActionTool&)>;
	using ActionToolActivatedSignalListener = ActionToolActivatedSignal::slot_function_type;

	boost::signals2::connection connectToActionToolActivatedSignal(ActionToolActivatedSignalListener listener);

public: // lucca::ToolGroup interface
	virtual bool addTool(lucca::Tool* tool) final override;
	virtual lucca::Tool* getActiveTool() const final override;
	virtual void activateDefaultTool() final override;

private:
	void _toolActivated(QAction* action);

private:
	std::unique_ptr<QActionGroup> _actionGroup;
	QAction* _defaultTool = nullptr;
	lucca::Tool* _activeTool = nullptr;
	ActionToolActivatedSignal _actionToolActivatedSignal;
};

}

#endif // LUCCA_QT_ACTION_TOOL_GROUP_HPP