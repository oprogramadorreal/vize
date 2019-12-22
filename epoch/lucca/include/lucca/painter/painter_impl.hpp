#ifndef LUCCA_PAINTER_IMPL_HPP
#define LUCCA_PAINTER_IMPL_HPP

#include "lucca/config.hpp"

#include "lucca/painter/painter.hpp"

namespace lucca {

/**
 * @author O Programador
 */
class LUCCA_API PainterImpl : public Painter {
public:
	PainterImpl();
	virtual ~PainterImpl();

protected:
	virtual void initialize(RenderTarget* target) final override;
	virtual void dispose() final override;
	virtual bool paint(const Camera& camera) final override;
	virtual void resize(SizeType width, SizeType height) final override;
	virtual bool compareRenderTarget(const RenderTarget* target) const final override;
	virtual bool hasCurrentRenderingContext() const final override;
	virtual ayla::AxisAlignedBox getAABB() const final override;
	virtual std::unique_ptr<Painter> clone() const final override;
	virtual void addSubPainter(std::unique_ptr<Painter> subPainter) final override;

private:
	virtual void _initializeImpl(RenderTarget* target) = 0;
	virtual void _disposeImpl() = 0;
	virtual void _paintImpl(const Camera& camera) = 0;
	virtual void _resizeImpl(SizeType width, SizeType height) = 0;
	virtual ayla::AxisAlignedBox _getAABBImpl() const = 0;
	virtual std::unique_ptr<Painter> _cloneImpl() const = 0;

private:
	RenderTarget* _renderTarget = nullptr;
};

}

#endif // LUCCA_PAINTER_IMPL_HPP