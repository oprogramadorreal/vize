#ifndef LUCCA_TOOL_HPP
#define LUCCA_TOOL_HPP

#include "lucca/config.hpp"

#include <boost/signals2.hpp>
#include <boost/noncopyable.hpp>

namespace lucca {

class ToolGroup;

/**
 * @author O Programador
 */
class LUCCA_API Tool : boost::noncopyable {
public:
	virtual ~Tool() = default;

	/**
	 * Activate this tool.
	 */
	virtual void activateTool() = 0;

	/**
	 * Deactivate this tool.
	 */
	virtual void deactivateTool() = 0;

	virtual bool isToolActive() const = 0;

	virtual void setToolGroup(ToolGroup*) = 0;

	using ActivatedSignal = boost::signals2::signal<void(Tool&)>;
	using ActivatedSignalListener = ActivatedSignal::slot_function_type;

	/**
	 * Register a listener to be notified when this tool is activated.
	 */
	virtual boost::signals2::connection connectToActivatedSignal(ActivatedSignalListener listener) = 0;
};

}

#endif // LUCCA_TOOL_HPP