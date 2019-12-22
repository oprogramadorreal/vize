#ifndef LUCCA_PAINTER_COMPOSITE_HPP
#define LUCCA_PAINTER_COMPOSITE_HPP

#include "lucca/config.hpp"

#include "lucca/painter/painter.hpp"

#include <map>
#include <vector>
#include <functional>

namespace lucca {

/**
 * @author O Programador
 */
class LUCCA_API PainterComposite final : public Painter {
public:
	PainterComposite();
	virtual ~PainterComposite();

public:
	void addSubPainterAt(std::unique_ptr<Painter> painter, SizeType paintingOrder);

	void removePaintersAt(SizeType paintingOrder);

	using PainterVisitor = std::function<void(Painter&)>;

	/**
	 * Visit all painters at @param paintingOrder.
	 */
	void visit(const PainterVisitor& visitor, SizeType paintingOrder) const;

	/**
	 * Visit all painters.
	 */
	void visit(const PainterVisitor& visitor) const;

	/**
	 * Used by lucca::DynamicTool.
	 */
	void setToolPainter(std::unique_ptr<lucca::Painter> painter);

	void setToolPainterOrder(SizeType order);

	/**
	 * @author O Programador
	 */
	enum class FilterAction {
		Paint = 0,
		PaintAfterOthersInSameOrder,
		PaintAfterEverything,
		DoNotPaint
	};

	/**
	 * Provides more flexibility on painting order using a filter which
	 * must analyze the painter and return one of the possible filter actions.
	 */
	bool paint(const Camera& camera, const std::function<FilterAction(const Painter& painter, SizeType paintingOrder)>& filter);

public: // lucca::Painter interface
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
	using PaintersList = std::vector<std::unique_ptr<Painter> >;
	std::map<SizeType, PaintersList> _painters;
	SizeType _toolPaintingOrder = SizeType(0);
	std::vector<Painter*> _paintAfterOthersInSameOrder;
	std::vector<Painter*> _paintAfterEverything;
};

}

#endif // LUCCA_PAINTER_COMPOSITE_HPP