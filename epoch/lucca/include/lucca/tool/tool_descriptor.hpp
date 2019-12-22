#ifndef LUCCA_TOOL_DESCRIPTOR_HPP
#define LUCCA_TOOL_DESCRIPTOR_HPP

#include "lucca/config.hpp"

#include <string>

namespace lucca {

/**
 * Tool descriptor interface.
 *
 * @see lucca::Tool
 *
 * @author O Programador
 */
class LUCCA_API ToolDescriptor {
public:
	virtual ~ToolDescriptor();

public:
	virtual std::string getName() const;
	virtual std::string getIcon() const;
	virtual std::string getBigIcon() const;
	virtual std::string getToolTip() const;
	virtual std::string getCursor() const;
	virtual std::string getShortcut() const;
};

}

/**
 * @remarks All descriptors must be declared inside this namespace.
 */
namespace descriptor_declaration {

/**
 * Specialize this template.
 *
 * @see lucca::ToolDescriptor
 *
 * @author O Programador
 */
template <class ToolType>
class ToolDescriptor : public lucca::ToolDescriptor { };

}

#endif // LUCCA_TOOL_DESCRIPTOR_HPP