#ifndef LUCCA_TOOL_GROUP_HPP
#define LUCCA_TOOL_GROUP_HPP

#include "lucca/config.hpp"

#include <boost/noncopyable.hpp>

namespace lucca {

class Tool;

/**
 * @see lucca::DynamicTool
 * 
 * @author O Programador
 */
class LUCCA_API ToolGroup : boost::noncopyable {
public:
	virtual ~ToolGroup() = default;

	virtual bool addTool(Tool* tool) = 0;

	virtual Tool* getActiveTool() const = 0;

	virtual void activateDefaultTool() = 0;
};

}

#endif // LUCCA_TOOL_GROUP_HPP