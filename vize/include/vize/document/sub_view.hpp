#ifndef VIZE_SUB_VIEW_HPP
#define VIZE_SUB_VIEW_HPP

#include "vize/config.hpp"

#include <lucca_qt/qt_mdi_sub_window_view.hpp>

namespace lucca {
	class TrackballCameraController;
}
namespace lucca_qt {
	class QtActionTool;
}

namespace vize {

class VolumeRaycaster;

class TransferFunctionEditorWidget;
class MPRConfigWidget;
class SampleStepWidget;

class SubViewState;

/**
 * @author O Programador
 */
class SubView final : public lucca_qt::QtMdiSubWindowView {
public:
	SubView(lucca_qt::QtOpenGLView& glWidget);
	virtual ~SubView();

public:
	void setDefaultCameraPosition();

	void setState(const SubViewState& state);
	SubViewState getState() const;

private: // lucca::View interface
	virtual std::unique_ptr<View> clone() const final override;

private:
	lucca_qt::QtActionTool* _defaultLookTool = nullptr;
	std::unique_ptr<VolumeRaycaster> _raycasterProxy;
	std::unique_ptr<lucca::TrackballCameraController> _trackball;

	lucca_qt::QtActionTool* _modelTransformerSwitchTool = nullptr;
	lucca_qt::QtActionTool* _lightSwitchTool = nullptr;
	TransferFunctionEditorWidget* _transferFunctionEditorWidget = nullptr;
	MPRConfigWidget* _mprConfigWidget = nullptr;
	SampleStepWidget* _sampleStepWidget = nullptr;

	friend class SubViewFactory;
};

}

#endif // VIZE_SUB_VIEW_HPP