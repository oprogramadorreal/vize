#include <GL/glew.h>

#include "ayla_gl/renderers/wireframe_box_renderer.hpp"

namespace ayla_gl {

WireframeBoxRenderer::WireframeBoxRenderer(const std::shared_ptr<SimpleProgram>& shader, const ayla::Box& box)
	: _renderer(shader)
{
	setBox(box);
}

void WireframeBoxRenderer::setBox(const ayla::Box& newBox) {
	_box = newBox;

	const auto edges = _box.getEdges();

	_renderer.clear();

	for (auto it = edges.begin(); it != edges.end(); ++it) {
		_renderer.add(*it);
	}
}

}