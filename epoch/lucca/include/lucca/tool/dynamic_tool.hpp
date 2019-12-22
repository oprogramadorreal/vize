#ifndef LUCCA_DYNAMIC_TOOL_HPP
#define LUCCA_DYNAMIC_TOOL_HPP

#include "lucca/config.hpp"

#include "lucca/tool/tool_impl.hpp"

namespace lucca {

class View;
class Painter;
class EventHandler;
class MultiViewDocument;

/**
 * Abstract class.
 *
 * A "dynamic tool" automatically activate itself when a "multi view document"
 * changes its current active view. However, note that this automatic activation
 * is performed only if this tool is added into a tool group.
 *
 * When a "dynamic tool" is activated, it sets unique event handlers and a
 * painter into the current active view.
 *
 * @see lucca::ToolGroup
 * @see lucca::ScreenPenTool
 *
 * @author O Programador
 */
class LUCCA_API DynamicTool : public ToolImpl {
public:
	DynamicTool(MultiViewDocument& document);
	virtual ~DynamicTool() = 0;

public:
	using EventHandlers = std::vector<EventHandler*>;

public:
	/**
	 * An unique painter for this tool.
	 */
	virtual std::unique_ptr<Painter> createToolPainter();

	/**
	 * @return The event handlers for this tool.
	 */
	virtual EventHandlers getToolEventHandlers();

	/**
	 * Defines the "event group" where the event handlers of this tool will be placed.
	 *
	 * @remarks Please call this before this tool is activated.
	 */
	void setEventHandlersGroup(SizeType eventHandlersGroup);

protected:
	MultiViewDocument& _getDocument() const;

private: // lucca::Tool interface
	virtual void _activateImpl() override;

	virtual bool isToolActive() const final override;
	virtual void setToolGroup(ToolGroup* toolGroup) final override;

private:
	void _subViewActivated(const View&);
	void _dynamicToolActivated(Tool&);

private:
	MultiViewDocument& _document;
	boost::signals2::scoped_connection _documentConnetion;
	boost::signals2::scoped_connection _toolConnetion;
	const ToolGroup* _toolGroup = nullptr;
	SizeType _eventHandlersGroup = SizeType(0);
};

}

#endif // LUCCA_DYNAMIC_TOOL_HPP