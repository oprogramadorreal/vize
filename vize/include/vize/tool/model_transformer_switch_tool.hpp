#ifndef VIZE_MODEL_TRANSFORMER_SWITCH_TOOL_HPP
#define VIZE_MODEL_TRANSFORMER_SWITCH_TOOL_HPP

#include "vize/config.hpp"

#include <lucca/tool/tool_impl.hpp>

namespace vize {

class ModelTransformerWidget;

/**
 * @author O Programador
 */
class ModelTransformerSwitchTool final : public lucca::ToolImpl {
public:
	ModelTransformerSwitchTool(ModelTransformerWidget& widget);
	virtual ~ModelTransformerSwitchTool();

private:
	virtual bool isToolActive() const final override;
	virtual void _activateImpl() final override;
	virtual void _deactivateImpl() final override;

private:
	ModelTransformerWidget& _widget;
};

}

#endif // VIZE_MODEL_TRANSFORMER_SWITCH_TOOL_HPP