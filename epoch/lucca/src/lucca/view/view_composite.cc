#include "lucca/view/view_composite.hpp"

#include "lucca/painter/painter.hpp"
#include "lucca/camera.hpp"

namespace lucca {

ViewComposite::~ViewComposite() = default;

void ViewComposite::setPainter(std::unique_ptr<Painter> painter) {
	if (const auto activeView = getActiveSubView()) {
		return activeView->setPainter(std::move(painter));
	}
}

Painter* ViewComposite::getPainter() const {
	if (const auto activeView = getActiveSubView()) {
		return activeView->getPainter();
	}

	return nullptr;
}

Camera* ViewComposite::getDefaultCamera() const {
	if (const auto activeView = getActiveSubView()) {
		return activeView->getDefaultCamera();
	}

	return nullptr;
}

void ViewComposite::addCamera(std::unique_ptr<Camera> camera) {
	if (const auto activeView = getActiveSubView()) {
		activeView->addCamera(std::move(camera));
	}
}

void ViewComposite::clearConnections(SizeType connectionsGroup) {
	if (const auto activeView = getActiveSubView()) {
		activeView->clearConnections(connectionsGroup);
	}
}

bool ViewComposite::connectEventHandler(EventHandler& handler, SizeType connectionsGroup) {
	if (const auto activeView = getActiveSubView()) {
		return activeView->connectEventHandler(handler, connectionsGroup);
	}

	return false;
}

bool ViewComposite::setEventDispatcher(const std::shared_ptr<EventDispatcher>& dispatcher) {
	if (const auto activeView = getActiveSubView()) {
		return activeView->setEventDispatcher(dispatcher);
	}

	return false;
}

ViewComposite* ViewComposite::getComposite() {
	return this;
}

bool ViewComposite::hasCurrentRenderingContext() const {
	if (const auto activeView = getActiveSubView()) {
		return activeView->hasCurrentRenderingContext();
	}

	return false;
}

void ViewComposite::makeCurrentRenderingContext() {
	if (const auto activeView = getActiveSubView()) {
		return activeView->makeCurrentRenderingContext();
	}
}

void ViewComposite::doneCurrentRenderingContext() {
	if (const auto activeView = getActiveSubView()) {
		return activeView->doneCurrentRenderingContext();
	}
}

SizeType ViewComposite::getDefaultFramebufferObject() const {
	if (const auto activeView = getActiveSubView()) {
		return activeView->getDefaultFramebufferObject();
	}

	return SizeType();
}

SizeType ViewComposite::getRenderTargetWidth() const {
	if (const auto activeView = getActiveSubView()) {
		return activeView->getRenderTargetWidth();
	}

	return SizeType();
}

SizeType ViewComposite::getRenderTargetHeight() const {
	if (const auto activeView = getActiveSubView()) {
		return activeView->getRenderTargetHeight();
	}

	return SizeType();
}

void ViewComposite::postRedisplay() {
	if (const auto activeView = getActiveSubView()) {
		return activeView->postRedisplay();
	}
}

}