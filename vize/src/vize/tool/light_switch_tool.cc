#include "vize/tool/light_switch_tool.hpp"
#include "vize/painter/volume_raycaster.hpp"

namespace vize {

LightSwitchTool::LightSwitchTool(VolumeRaycaster& raycaster)
	: _raycaster(raycaster)
{ }

LightSwitchTool::~LightSwitchTool() = default;

bool LightSwitchTool::isToolActive() const {
	return _raycaster.getLightingEnabled();
}

void LightSwitchTool::_activateImpl() {
	_raycaster.setLightingEnabled(true);
}

void LightSwitchTool::_deactivateImpl() {
	_raycaster.setLightingEnabled(false);
}

}