#include "lucca/viewport.hpp"
#include "lucca/render_target.hpp"

namespace lucca {

Viewport::Viewport() = default;

Viewport::Viewport(Float relativeX, Float relativeY, Float relativeWidth, Float relativeHeight) {
	setRelativeX(relativeX);
	setRelativeY(relativeY);
	setRelativeWidth(relativeWidth);
	setRelativeHeight(relativeHeight);
}

Viewport::Viewport(Float relativeX, Float relativeY, Float relativeWidth, Float relativeHeight, RenderTarget* renderTarget)
	: Viewport(relativeX, relativeY, relativeWidth, relativeHeight)
{
	setRenderTarget(renderTarget);
}

Viewport::~Viewport() = default;

Float Viewport::getRelativeX() const {
	return _relativeX;
}

Float Viewport::getRelativeY() const {
	return _relativeY;
}

Float Viewport::getRelativeWidth() const {
	return _relativeWidth;
}

Float Viewport::getRelativeHeight() const {
	return _relativeHeight;
}

SizeType Viewport::getAbsoluteX() const {
	assert(_renderTarget);
	return SizeType(getRelativeX() * _renderTarget->getRenderTargetWidth());
}

SizeType Viewport::getAbsoluteY() const {
	assert(_renderTarget);
	return SizeType(getRelativeY() * _renderTarget->getRenderTargetHeight());
}

SizeType Viewport::getAbsoluteWidth() const {
	assert(_renderTarget);
	return SizeType(getRelativeWidth() * _renderTarget->getRenderTargetWidth());
}

SizeType Viewport::getAbsoluteHeight() const {
	assert(_renderTarget);
	return SizeType(getRelativeHeight() * _renderTarget->getRenderTargetHeight());
}

void Viewport::setRelativeX(Float value) {
	_relativeX = glm::clamp(value, 0.0f, 1.0f);
}

void Viewport::setRelativeY(Float value) {
	_relativeY = glm::clamp(value, 0.0f, 1.0f);
}

void Viewport::setRelativeWidth(Float value) {
	_relativeWidth = glm::clamp(value, 0.0f, 1.0f);
}

void Viewport::setRelativeHeight(Float value) {
	_relativeHeight = glm::clamp(value, 0.0f, 1.0f);
}

void Viewport::setRenderTarget(RenderTarget* renderTarget) {
	_renderTarget = renderTarget;
}

SizeType Viewport::getDefaultFramebufferObject() const {
	assert(_renderTarget);
	return _renderTarget->getDefaultFramebufferObject();
}

void Viewport::postRedisplay() {
	assert(_renderTarget);
	_renderTarget->postRedisplay();
}

}