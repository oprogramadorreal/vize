#include <GL/glew.h>

#include "lucca/painter/simple_triangle_mesh_painter.hpp"

#include "lucca/camera.hpp"

#include <ayla_gl/renderers/triangle_mesh_renderer.hpp>
#include <ayla_gl/renderers/wireframe_box_renderer.hpp>
#include <ayla_gl/shaders/phong_shading_program.hpp>

namespace lucca {

SimpleTriangleMeshPainter::SimpleTriangleMeshPainter(const std::shared_ptr<ayla::TriangleMesh>& mesh, const glm::mat4& modelMatrix)
	: _mesh(mesh), _modelMatrix(modelMatrix)
{ }

SimpleTriangleMeshPainter::~SimpleTriangleMeshPainter() = default;

void SimpleTriangleMeshPainter::_initializeImpl(RenderTarget* target) {
	if (_mesh) {
		_meshGL = std::make_unique<ayla_gl::TriangleMeshRenderer>(*_mesh, GL_STATIC_DRAW);
		_meshAabbGL = std::make_unique<ayla_gl::WireframeBoxRenderer>(getAABB());
	}

	_phongShaderGL = std::make_unique<ayla_gl::PhongShadingProgram>();

	glClearColor(0.7f, 0.7f, 0.7f, 0.0f); // background color
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glDepthFunc(GL_LESS); // accept fragment if it closer to the camera than the former one
}

void SimpleTriangleMeshPainter::_disposeImpl() {
	_meshGL.reset();
	_meshAabbGL.reset();
	_phongShaderGL.reset();
}

void SimpleTriangleMeshPainter::_paintImpl(const Camera& camera) {
	if (!_meshGL) {
		return;
	}

	assert(_phongShaderGL);

	const auto view = camera.getViewMatrix();
	const auto projection = camera.getProjectionMatrix();
	const auto lightPosition = camera.getPosition();

	glPushAttrib(GL_POLYGON_BIT | GL_LINE_BIT | GL_ENABLE_BIT);

	glUseProgram(_phongShaderGL->getId());

	glUniform3f(_phongShaderGL->getUniform_lightPosition_worldspace(), lightPosition.x, lightPosition.y, lightPosition.z);
	glUniform1f(_phongShaderGL->getUniform_lightPower(), 1.2f);
	glUniform1i(_phongShaderGL->getUniform_lightQuadraticAttenuation(), false);
	glUniformMatrix4fv(_phongShaderGL->getUniform_M(), 1, GL_FALSE, &_modelMatrix[0][0]);
	glUniformMatrix4fv(_phongShaderGL->getUniform_V(), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(_phongShaderGL->getUniform_P(), 1, GL_FALSE, &projection[0][0]);
	glUniform4f(_phongShaderGL->getUniform_materialDiffuseColor(), 0.0f, 0.0f, 1.0f, 1.0f); // blue mesh
	glUniform4f(_phongShaderGL->getUniform_materialSpecularColor(), 0.6f, 0.6f, 0.6f, 1.0f);
	_meshGL->drawElements();

	glUseProgram(0);

	if (_meshAabbGL) {
		_meshAabbGL->draw(view, projection);
	}

	glPopAttrib();
}

void SimpleTriangleMeshPainter::_resizeImpl(SizeType, SizeType) {
	// nothing to do
}

ayla::AxisAlignedBox SimpleTriangleMeshPainter::_getAABBImpl() const {
	auto aabb = _mesh ? _mesh->calculateAABB() : ayla::AxisAlignedBox(glm::vec3(-0.5f), glm::vec3(0.5f));
	aabb.transform(_modelMatrix);
	return aabb;
}

std::unique_ptr<Painter> SimpleTriangleMeshPainter::_cloneImpl() const {
	return std::make_unique<SimpleTriangleMeshPainter>(_mesh, _modelMatrix);
}

}