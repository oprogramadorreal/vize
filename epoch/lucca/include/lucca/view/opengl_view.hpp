#ifndef LUCCA_OPENGL_VIEW_HPP
#define LUCCA_OPENGL_VIEW_HPP

#include "lucca/config.hpp"
#include "lucca/view/view.hpp"

#include <vector>

namespace lucca {

/**
 * Partial implementation of lucca::View interface using OpenGL.
 *
 * @remarks This is is still an abstract class because it does not
 * implement lucca::RenderTarget interface.
 *
 * @see lucca::View
 *
 * @author O Programador
 */
class LUCCA_API OpenGLView : public View {
public:
	OpenGLView();
	virtual ~OpenGLView();

private: // lucca::View interface
	virtual void setPainter(std::unique_ptr<Painter> painter) final override;
	virtual Painter* getPainter() const final override;
	virtual Camera* getDefaultCamera() const final override;
	virtual void addCamera(std::unique_ptr<Camera> camera) final override;

protected:
	void _copyFrom(const OpenGLView& other);

protected:
	void _initialize();
	void _dispose();
	void _resize(SizeType width, SizeType height);
	void _paint();

protected:
	// callable from contructor:
	void _setPainter(std::unique_ptr<Painter> painter);
	void _addCamera(std::unique_ptr<Camera> camera);

private:
	std::unique_ptr<Painter> _painter;
	std::vector<std::unique_ptr<Camera> > _cameras;
};

}

#endif // LUCCA_OPENGL_VIEW_HPP