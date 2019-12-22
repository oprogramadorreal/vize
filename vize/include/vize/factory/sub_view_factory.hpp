#ifndef VIZE_SUB_VIEW_FACTORY_HPP
#define VIZE_SUB_VIEW_FACTORY_HPP

#include "vize/config.hpp"

class QToolBar;

namespace lucca {
	class TrackballCameraController;
}
namespace lucca_qt {
	class QtOpenGLView;
}

namespace vize {

class SubView;

class TransferFunctionEditorWidget;
class MPRConfigWidget;
class SampleStepWidget;
class ModelTransformerWidget;

class VolumeRaycaster;

/**
 * @author O Programador
 */
class SubViewFactory final {
public:
	std::unique_ptr<SubView> create(std::unique_ptr<lucca_qt::QtOpenGLView> glWidget) const;

private:
	void _assembleLayout(
		SubView& documentSubView,
		std::unique_ptr<lucca_qt::QtOpenGLView> glWidget,
		std::unique_ptr<QToolBar> toolBar,
		TransferFunctionEditorWidget* transferFunctionWidget,
		MPRConfigWidget* mprConfigWidget,
		ModelTransformerWidget* modelTransformerWidget
	) const;

	std::unique_ptr<lucca::TrackballCameraController> _createTrackball(SubView& documentSubView) const;
	std::unique_ptr<QToolBar> _createToolBar(SubView& documentSubView, SampleStepWidget* sampleStepWidget, VolumeRaycaster& rayCaster, ModelTransformerWidget& modelTransformerWidget) const;
};

}

#endif // VIZE_SUB_VIEW_FACTORY_HPP