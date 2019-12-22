#include "vize/factory/sub_view_factory.hpp"

#include "vize/application/ui_colors.hpp"

#include "vize/document/sub_view.hpp"
#include "vize/painter/volume_painter_proxy.hpp"

#include "vize/ui/transfer_function_editor_widget.hpp"
#include "vize/ui/mpr_config_widget.hpp"
#include "vize/ui/sample_step_widget.hpp"
#include "vize/ui/model_transformer_widget.hpp"

#include "vize/tool/descriptors.hpp"
#include "vize/tool/light_switch_tool.hpp"
#include "vize/tool/model_transformer_switch_tool.hpp"

#include <lucca/tool/look_at/look_front_tool.hpp>
#include <lucca/tool/look_at/look_back_tool.hpp>
#include <lucca/tool/look_at/look_left_tool.hpp>
#include <lucca/tool/look_at/look_right_tool.hpp>
#include <lucca/tool/look_at/look_top_tool.hpp>
#include <lucca/tool/look_at/look_bottom_tool.hpp>
#include <lucca/tool/duplicate_sub_view_tool.hpp>

#include <lucca/event/trackball_camera_controller.hpp>

#include <lucca_qt/make_qt_action_tool.hpp>
#include <lucca_qt/qt_opengl_view.hpp>

#include <QToolBar>
#include <QToolButton>
#include <QAction>
#include <QMenu>
#include <QVBoxLayout>

namespace vize {

std::unique_ptr<SubView> SubViewFactory::create(std::unique_ptr<lucca_qt::QtOpenGLView> glWidget) const {
	assert(glWidget);
	auto documentSubView = std::make_unique<SubView>(*glWidget);

	auto raycasterProxy = std::make_unique<VolumePainterProxy>(*documentSubView);

	const auto uiA = std::make_unique<TransferFunctionEditorWidget>(*raycasterProxy, documentSubView.get()).release();
	const auto uiB = std::make_unique<MPRConfigWidget>(*raycasterProxy, documentSubView.get()).release();
	const auto uiC = std::make_unique<SampleStepWidget>(*raycasterProxy, documentSubView.get()).release();
	const auto uiD = std::make_unique<ModelTransformerWidget>(*raycasterProxy, documentSubView.get()).release();

	auto toolBar = _createToolBar(*documentSubView, uiC, *raycasterProxy, *uiD);
	_assembleLayout(*documentSubView, std::move(glWidget), std::move(toolBar), uiA, uiB, uiD);
	auto trackball = _createTrackball(*documentSubView);

	documentSubView->_raycasterProxy = std::move(raycasterProxy);
	documentSubView->_trackball = std::move(trackball);
	documentSubView->_transferFunctionEditorWidget = uiA;
	documentSubView->_mprConfigWidget = uiB;
	documentSubView->_sampleStepWidget = uiC;

	return documentSubView;
}

void SubViewFactory::_assembleLayout(
	SubView& documentSubView,
	std::unique_ptr<lucca_qt::QtOpenGLView> glWidget,
	std::unique_ptr<QToolBar> toolBar,
	TransferFunctionEditorWidget* transferFunctionWidget,
	MPRConfigWidget* mprConfigWidget,
	ModelTransformerWidget* modelTransformerWidget
) const {
	auto dummyWidget = std::make_unique<QWidget>(&documentSubView).release();

	auto verticalLayout = std::make_unique<QVBoxLayout>(dummyWidget).release();
	verticalLayout->setSpacing(0);
	verticalLayout->setContentsMargins(0, 0, 0, 0);

	auto configsWidget = std::make_unique<QWidget>(&documentSubView).release();
	auto horizontalLayout = std::make_unique<QHBoxLayout>(configsWidget).release();
	horizontalLayout->setSpacing(0);
	horizontalLayout->setContentsMargins(0, 0, 0, 0);
	auto configsWidgetSizePolicy = configsWidget->sizePolicy();
	configsWidgetSizePolicy.setVerticalStretch(1);
	configsWidget->setSizePolicy(configsWidgetSizePolicy);
	QPalette pal(configsWidget->palette());
	const auto backgroundColor = UiColors::getTransferFunctionBarColor();
	pal.setColor(QPalette::Background, QColor(backgroundColor.x, backgroundColor.y, backgroundColor.z));
	configsWidget->setAutoFillBackground(true);
	configsWidget->setPalette(pal);
				
	horizontalLayout->addWidget(mprConfigWidget);		
	horizontalLayout->addWidget(transferFunctionWidget);
	horizontalLayout->addWidget(modelTransformerWidget);
	modelTransformerWidget->setVisible(false);
	
	auto widgetViewSizePolicy = glWidget->sizePolicy();
	widgetViewSizePolicy.setVerticalStretch(10);
	glWidget->setSizePolicy(widgetViewSizePolicy);

	verticalLayout->addWidget(glWidget.release());
	verticalLayout->addWidget(configsWidget);
	verticalLayout->addWidget(toolBar.release());
	
	documentSubView.setWidget(dummyWidget);
}

std::unique_ptr<lucca::TrackballCameraController> SubViewFactory::_createTrackball(SubView& documentSubView) const {
	auto trackball = std::unique_ptr<lucca::TrackballCameraController>();
	auto& widgetView = documentSubView.getWidgetView();

	if (const auto camera = widgetView.getDefaultCamera()) {
		trackball = std::make_unique<lucca::TrackballCameraController>(*camera);
		trackball->addRotationButton(lucca::MouseButton::Left);

		if (documentSubView._defaultLookTool) {
			if (const auto lookAtTool = dynamic_cast<lucca::LookAtTool*>(documentSubView._defaultLookTool->getActualTool())) {
				const auto cameraTargetFunction = std::bind(
					&lucca::LookAtTool::getCameraTarget, lookAtTool
				);

				trackball->setCameraTarget(cameraTargetFunction);
			}
		}

		widgetView.connectEventHandler(*trackball, SizeType(1));
	}

	return std::move(trackball);
}

std::unique_ptr<QToolBar> SubViewFactory::_createToolBar(SubView& documentSubView, SampleStepWidget* sampleStepWidget, VolumeRaycaster& rayCaster, ModelTransformerWidget& modelTransformerWidget) const {
	auto duplicateViewAction = lucca_qt::makeQtActionTool<lucca::DuplicateSubViewTool>(&documentSubView, documentSubView).release();
	auto lookFrontAction = lucca_qt::makeQtActionTool<lucca::LookFrontTool>(&documentSubView, documentSubView).release();
	auto lookBackAction = lucca_qt::makeQtActionTool<lucca::LookBackTool>(&documentSubView, documentSubView).release();
	auto lookLeftAction = lucca_qt::makeQtActionTool<lucca::LookLeftTool>(&documentSubView, documentSubView).release();
	auto lookRightAction = lucca_qt::makeQtActionTool<lucca::LookRightTool>(&documentSubView, documentSubView).release();
	auto lookTopAction = lucca_qt::makeQtActionTool<lucca::LookTopTool>(&documentSubView, documentSubView).release();
	auto lookBottomAction = lucca_qt::makeQtActionTool<lucca::LookBottomTool>(&documentSubView, documentSubView).release();

	auto lookAtMenu = std::make_unique<QMenu>(&documentSubView).release();
	lookAtMenu->addAction(lookFrontAction);
	lookAtMenu->addAction(lookBackAction);
	lookAtMenu->addAction(lookLeftAction);
	lookAtMenu->addAction(lookRightAction);
	lookAtMenu->addAction(lookTopAction);
	lookAtMenu->addAction(lookBottomAction);

	auto lookAtButton = std::make_unique<QToolButton>(&documentSubView).release();
	lookAtButton->setMenu(lookAtMenu);
	lookAtButton->setPopupMode(QToolButton::InstantPopup);
	lookAtButton->setToolTip("Look at...");
	lookAtButton->setIcon(QIcon(":resources/icons/16/eye"));

	auto toolBar = std::make_unique<QToolBar>(&documentSubView);
	toolBar->setMinimumHeight(25);
	toolBar->setMaximumHeight(25);
	toolBar->setIconSize(QSize(16, 16));

	toolBar->addAction(duplicateViewAction);
	toolBar->addWidget(lookAtButton);

	toolBar->addSeparator();

	auto lightSwitchAction = lucca_qt::makeQtActionTool<LightSwitchTool>(&documentSubView, rayCaster).release();
	lightSwitchAction->setCheckable(true);
	toolBar->addAction(lightSwitchAction);
	toolBar->addWidget(sampleStepWidget);
	documentSubView._lightSwitchTool = lightSwitchAction;

	toolBar->addSeparator();

	auto modelTransformerSwitchAction = lucca_qt::makeQtActionTool<ModelTransformerSwitchTool>(&documentSubView, modelTransformerWidget).release();
	modelTransformerSwitchAction->setCheckable(true);
	toolBar->addAction(modelTransformerSwitchAction);
	documentSubView._modelTransformerSwitchTool = modelTransformerSwitchAction;

	return toolBar;
}

}