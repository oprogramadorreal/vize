#ifndef AYLA_GL_LINES_RENDERER_HPP
#define AYLA_GL_LINES_RENDERER_HPP

#include "ayla_gl/config.hpp"

#include "ayla_gl/shaders/simple_program.hpp"
#include <ayla/geometry/line_segment.hpp>

#include <vector>

namespace ayla_gl {

/**
 * Draw line segments.
 *
 * @see ayla::LineSegment
 *
 * @author O Programador
 */
class AYLA_GL_API LinesRenderer final : boost::noncopyable {
public:
	inline LinesRenderer(const std::vector<ayla::LineSegment>& lines = std::vector<ayla::LineSegment>())
		: LinesRenderer(std::make_shared<SimpleProgram>(), lines)
	{ }

	LinesRenderer(const std::shared_ptr<SimpleProgram>& shader, const std::vector<ayla::LineSegment>& lines = std::vector<ayla::LineSegment>());

	~LinesRenderer();

	inline SizeType getNumberOfLines() const {
		return SizeType(_lines.size());
	}

	inline const ayla::LineSegment& getLine(SizeType index) const {
		assert(index < getNumberOfLines());
		return _lines[index];
	}
	
	/**
	 * Affects the drawing of this Line.
	 * If true, the line will be dashed. - - - - -
	 */
	inline void setDashed(bool dashed) {
		_dashed = dashed;
	}
	
	inline void setLine(SizeType index, const ayla::LineSegment& newLine) {
		assert(index < getNumberOfLines());
		_lines[index] = newLine;
		_needsUpdate = true;
	}

	inline void add(const ayla::LineSegment& line) {
		_lines.push_back(line);
		_needsUpdate = true;
	}

	inline void clear() {
		_lines.clear();
		_needsUpdate = true;
	}

	inline const std::shared_ptr<SimpleProgram>& getShader() const { return _shader; }

	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) const;

private:
	void _updateVBO() const;

private:
	GLuint _vao = 0u; // vertex array object
	GLuint _vbo = 0u; // vertex buffer object
	std::shared_ptr<SimpleProgram> _shader;
	std::vector<ayla::LineSegment> _lines;
	mutable bool _needsUpdate = true;
	//If set to true, the line will be dashed when drawn
	bool _dashed = false;
};

}

#endif // AYLA_GL_LINES_RENDERER_HPP