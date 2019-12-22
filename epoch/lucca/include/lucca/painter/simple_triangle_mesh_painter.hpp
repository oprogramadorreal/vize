#ifndef LUCCA_TRIANGLE_MESH_PAINTER_HPP
#define LUCCA_TRIANGLE_MESH_PAINTER_HPP

#include "lucca/config.hpp"

#include "lucca/painter/painter_impl.hpp"

namespace ayla {
	class TriangleMesh;
	class AxisAlignedBox;
}
namespace ayla_gl {
	class TriangleMeshRenderer;
	class PhongShadingProgram;
	class WireframeBoxRenderer;
}

namespace lucca {

/**
 * Basic Painter implementation that shows how to draw an ayla::TriangleMesh.
 *
 * @see lucca::Painter
 * @see ayla::TriangleMesh
 *
 * @author O Programador
 */
class LUCCA_API SimpleTriangleMeshPainter final : public PainterImpl {
public:
	SimpleTriangleMeshPainter(const std::shared_ptr<ayla::TriangleMesh>& mesh, const glm::mat4& modelMatrix = glm::mat4(1.0f));
	virtual ~SimpleTriangleMeshPainter();

private: // lucca::PainterImpl interface
	virtual void _initializeImpl(RenderTarget* target) final override;
	virtual void _disposeImpl() final override;
	virtual void _paintImpl(const Camera& camera) final override;
	virtual void _resizeImpl(SizeType width, SizeType height) final override;
	virtual ayla::AxisAlignedBox _getAABBImpl() const final override;
	virtual std::unique_ptr<Painter> _cloneImpl() const final override;

private:
	const std::shared_ptr<ayla::TriangleMesh> _mesh;
	std::unique_ptr<ayla_gl::TriangleMeshRenderer> _meshGL;
	std::unique_ptr<ayla_gl::WireframeBoxRenderer> _meshAabbGL;
	std::unique_ptr<ayla_gl::PhongShadingProgram> _phongShaderGL;
	glm::mat4 _modelMatrix;
};

}

#endif // LUCCA_TRIANGLE_MESH_PAINTER_HPP