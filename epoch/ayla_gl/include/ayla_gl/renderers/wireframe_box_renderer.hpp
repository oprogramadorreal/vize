#ifndef AYLA_GL_WIREFRAME_BOX_RENDERER_HPP
#define AYLA_GL_WIREFRAME_BOX_RENDERER_HPP

#include "ayla_gl/config.hpp"

#include "ayla_gl/renderers/lines_renderer.hpp"
#include <ayla/geometry/box.hpp>

namespace ayla_gl {

/**
 * Draw a box in wireframe.
 *
 * @see ayla::Box
 * @see ayla::AxisAlignedBox
 *
 * @author O Programador
 */
class AYLA_GL_API WireframeBoxRenderer final : boost::noncopyable {
public:
	inline WireframeBoxRenderer(const ayla::Box& box = ayla::AxisAlignedBox(glm::vec3(0.0f, 0.0f, 0.0f), 0.5f, 0.5f, 0.5f))
		: WireframeBoxRenderer(std::make_shared<SimpleProgram>(), box)
	{ }

	/**
	 * Default: axis aligned box with unit volume centered on (0,0,0).
	 */
	WireframeBoxRenderer(const std::shared_ptr<SimpleProgram>& shader, const ayla::Box& box = ayla::AxisAlignedBox(glm::vec3(0.0f, 0.0f, 0.0f), 0.5f, 0.5f, 0.5f));

	inline const std::shared_ptr<SimpleProgram>& getShader() const { return _renderer.getShader(); }

	void setBox(const ayla::Box& newBox);
	inline const ayla::Box& getBox() const { return _box; }

	inline void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) const {
		_renderer.draw(viewMatrix, projectionMatrix);
	}

private:
	ayla::Box _box;
	LinesRenderer _renderer;
};

}

#endif // AYLA_GL_WIREFRAME_BOX_RENDERER_HPP