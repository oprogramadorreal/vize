#include "vize/document/sub_view.hpp"

#include "vize/document/sub_view_state.hpp"
#include "vize/factory/sub_view_factory.hpp"

#include "vize/painter/volume_raycaster.hpp"

#include "vize/tool/light_switch_tool.hpp"
#include "vize/tool/model_transformer_switch_tool.hpp"

#include "vize/ui/transfer_function_editor_widget.hpp"
#include "vize/ui/mpr_config_widget.hpp"
#include "vize/ui/sample_step_widget.hpp"

#include <lucca/painter/painter.hpp>
#include <lucca/event/trackball_camera_controller.hpp>
#include <lucca_qt/qt_action_tool.hpp>
#include <lucca_qt/qt_opengl_view.hpp>

#include <ayla/dynamic_unique_cast.hpp>

namespace vize {

SubView::SubView(lucca_qt::QtOpenGLView& glWidget)
	: QtMdiSubWindowView(glWidget)
{ }

SubView::~SubView() = default;

void SubView::setDefaultCameraPosition() {
	if (_defaultLookTool) {
		_defaultLookTool->trigger();
	}
}

std::unique_ptr<lucca::View> SubView::clone() const {
	auto copy = SubViewFactory().create(
		ayla::dynamic_unique_cast<lucca_qt::QtOpenGLView>(getWidgetView().clone())
	);

	if (_trackball) {
		copy->_trackball->copyCameraFrom(*_trackball);
	}

	copy->setState(getState());

	return std::move(copy);
}

namespace {

void _setToolActive(lucca::Tool& tool, bool active) {
	if (active) {
		tool.activateTool();
	} else {
		tool.deactivateTool();
	}
}

}

void SubView::setState(const SubViewState& state) {
	if (_modelTransformerSwitchTool) {
		_setToolActive(*_modelTransformerSwitchTool, state.getShowModelTransformer());
	}
	if (_lightSwitchTool) {
		_setToolActive(*_lightSwitchTool, state.getLightingEnabled());
	}
	if (_transferFunctionEditorWidget) {
		_transferFunctionEditorWidget->setTransferFunction(state.getTransferFunction());
	}
	if (_mprConfigWidget) {
		_mprConfigWidget->setXPlanePosition(state.getXPlanePosition());
		_mprConfigWidget->setYPlanePosition(state.getYPlanePosition());
		_mprConfigWidget->setZPlanePosition(state.getZPlanePosition());
		_mprConfigWidget->setCuttingPlanesEnabled(state.getCuttingPlanesEnabled());
	}
	if (_sampleStepWidget) {
		_sampleStepWidget->setSampleStep(state.getSampleStep());
	}
}

SubViewState SubView::getState() const {
	SubViewState state;

	if (_modelTransformerSwitchTool) {
		state.setShowModelTransformer(_modelTransformerSwitchTool->isToolActive());
	}
	if (_lightSwitchTool) {
		state.setLightingEnabled(_lightSwitchTool->isToolActive());
	}
	if (_transferFunctionEditorWidget) {
		state.setTransferFunction(_transferFunctionEditorWidget->getTransferFunction());
	}
	if (_mprConfigWidget) {
		state.setXPlanePosition(_mprConfigWidget->getXPlanePosition());
		state.setYPlanePosition(_mprConfigWidget->getYPlanePosition());
		state.setZPlanePosition(_mprConfigWidget->getZPlanePosition());
		state.setCuttingPlanesEnabled(_mprConfigWidget->getCuttingPlanesEnabled());
	}
	if (_sampleStepWidget) {
		state.setSampleStep(_sampleStepWidget->getSampleStep());
	}

	return state;
}

}