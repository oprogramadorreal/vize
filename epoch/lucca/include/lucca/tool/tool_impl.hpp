#ifndef LUCCA_TOOL_IMPL_HPP
#define LUCCA_TOOL_IMPL_HPP

#include "lucca/config.hpp"

#include "lucca/tool/tool.hpp"

namespace lucca {

/**
 * @author O Programador
 */
class LUCCA_API ToolImpl : public Tool {
public:
	ToolImpl();
	virtual ~ToolImpl();

protected:
	virtual void activateTool() final override;
	virtual void deactivateTool() final override;

	virtual boost::signals2::connection connectToActivatedSignal(ActivatedSignalListener listener) final override;

private: // lucca::Tool interface (you can override)
	virtual bool isToolActive() const override;
	virtual void setToolGroup(ToolGroup*) override;

private:
	virtual void _activateImpl() = 0;
	virtual void _deactivateImpl();

private:
	ActivatedSignal _activated;
};

}

#endif // LUCCA_TOOL_IMPL_HPP