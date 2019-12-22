#ifndef LUCCA_TOOL_BOX_HPP
#define LUCCA_TOOL_BOX_HPP

#include "lucca/config.hpp"

#include <boost/noncopyable.hpp>

namespace lucca {

class Tool;

/**
 * @see lucca::DynamicTool
 * 
 * @author O Programador
 */
class LUCCA_API ToolBox : boost::noncopyable {
public:
	virtual ~ToolBox() = default;

	virtual Tool* getActiveTool() const = 0;
};

}

#endif // LUCCA_TOOL_BOX_HPP