#ifndef VIZE_LIGHT_SWITCH_TOOL_HPP
#define VIZE_LIGHT_SWITCH_TOOL_HPP

#include "vize/config.hpp"

#include <lucca/tool/tool_impl.hpp>

namespace vize {

class VolumeRaycaster;

/**
 * @author O Programador
 */
class LightSwitchTool final : public lucca::ToolImpl {
public:
	LightSwitchTool(VolumeRaycaster& raycaster);
	virtual ~LightSwitchTool();

private:
	virtual bool isToolActive() const final override;
	virtual void _activateImpl() final override;
	virtual void _deactivateImpl() final override;

private:
	VolumeRaycaster& _raycaster;
};

}

#endif // VIZE_LIGHT_SWITCH_TOOL_HPP