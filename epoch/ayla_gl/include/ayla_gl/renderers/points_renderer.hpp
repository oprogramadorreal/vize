#ifndef AYLA_GL_POINTS_RENDERER_HPP
#define AYLA_GL_POINTS_RENDERER_HPP

#include "ayla_gl/config.hpp"

#include "ayla_gl/shaders/simple_program.hpp"

#include <vector>

namespace ayla_gl {

/**
 * Draw line segments.
 *
 * @see ayla::LineSegment
 *
 * @author O Programador
 */
class AYLA_GL_API PointsRenderer final : boost::noncopyable {
public:
	inline PointsRenderer(const std::vector<glm::vec3>& points = std::vector<glm::vec3>())
		: PointsRenderer(std::make_shared<SimpleProgram>(), points)
	{ }

	PointsRenderer(const std::shared_ptr<SimpleProgram>& shader, const std::vector<glm::vec3>& points = std::vector<glm::vec3>());

	~PointsRenderer();

	inline const std::vector<glm::vec3>& getPoints() const {
		return _points;
	}

	inline SizeType getNumberOfPoints() const {
		return SizeType(_points.size());
	}

	inline const glm::vec3& getPoint(SizeType index) const {
		assert(index < getNumberOfPoints());
		return _points[index];
	}
	
	inline void setPoint(SizeType index, const glm::vec3& newPoint) {
		assert(index < getNumberOfPoints());
		_points[index] = newPoint;
		_needsUpdate = true;
	}

	inline void add(const glm::vec3& newPoint) {
		_points.push_back(newPoint);
		_needsUpdate = true;
	}

	inline void clear() {
		_points.clear();
		_needsUpdate = true;
	}

	inline const std::shared_ptr<SimpleProgram>& getShader() const { return _shader; }

	/**
	 * Draw a sub set of this set of points.
	 */
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, SizeType numberOfPoints) const;

	/**
	 * Draw the set of points that were added into this renderer.
	 */
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) const;

	/**
	 * Draw lines connecting the points in this renderer in the order that they were added.
	 */
	void drawLineStrip(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) const;

private:
	void _updateVBO() const;

	void _drawBegin(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) const;
	void _drawEnd() const;

private:
	GLuint _vao = 0u; // vertex array object
	GLuint _vbo = 0u; // vertex buffer object
	std::shared_ptr<SimpleProgram> _shader;
	std::vector<glm::vec3> _points;
	mutable bool _needsUpdate = true;
};

}

#endif // AYLA_GL_POINTS_RENDERER_HPP