#include "lucca/painter/painter_impl.hpp"

#include "lucca/render_target.hpp"

#include <ayla/geometry/axis_aligned_box.hpp>

namespace lucca {

PainterImpl::PainterImpl() = default;

PainterImpl::~PainterImpl() {
	if (nullptr != _renderTarget) {
		AYLA_DEBUG_MESSAGE("lucca::PainterImpl::~PainterImpl() - WARNING: You have forgot to call Painter::dispose(). Potential GPU memory leak.");
	}
}

void PainterImpl::initialize(RenderTarget* target) {
	assert(target);

	dispose(); // dispose if already initialized

	_renderTarget = target;
	_renderTarget->makeCurrentRenderingContext();
	_initializeImpl(_renderTarget);
}

void PainterImpl::dispose() {
	if (_renderTarget) {
		_renderTarget->makeCurrentRenderingContext();
		_disposeImpl();
		_renderTarget = nullptr;
	}
}

bool PainterImpl::paint(const Camera& camera) {
	if (_renderTarget) {
		_renderTarget->makeCurrentRenderingContext();
		_paintImpl(camera);
	}

	return true;
}

void PainterImpl::resize(SizeType width, SizeType height)  {
	if (_renderTarget) {
		_renderTarget->makeCurrentRenderingContext();
		_resizeImpl(width, height);
	}
}

bool PainterImpl::compareRenderTarget(const RenderTarget* target) const {
	return _renderTarget == target;
}

bool PainterImpl::hasCurrentRenderingContext() const {
	return _renderTarget && _renderTarget->hasCurrentRenderingContext();
}

ayla::AxisAlignedBox PainterImpl::getAABB() const {
	return _getAABBImpl();
}

std::unique_ptr<Painter> PainterImpl::clone() const {
	return _cloneImpl();
}

void PainterImpl::addSubPainter(std::unique_ptr<Painter>) {
	assert(false);
}

}