#include "lucca/painter/painter_composite.hpp"

#include <ayla/geometry/axis_aligned_box.hpp>

namespace lucca {

PainterComposite::PainterComposite() = default;
PainterComposite::~PainterComposite() = default;

void PainterComposite::addSubPainterAt(std::unique_ptr<Painter> painter, SizeType paintingOrder) {
	_painters[paintingOrder].push_back(std::move(painter));
}

void PainterComposite::removePaintersAt(SizeType paintingOrder) {
	const auto it = _painters.find(paintingOrder);

	if (it == _painters.end()) {
		return; // nothing at @param paintingOrder position
	}

	for (const auto& p : it->second) {
		p->dispose();
	}

	_painters.erase(it);
}

void PainterComposite::visit(const PainterVisitor& visitor, SizeType paintingOrder) const {
	const auto it = _painters.find(paintingOrder);

	if (it == _painters.end()) {
		return; // nothing at @param paintingOrder position
	}

	for (const auto& p : it->second) {
		visitor(*p);
	}
}

void PainterComposite::visit(const PainterVisitor& visitor) const {
	for (const auto& order : _painters) {
		for (const auto& p : order.second) {
			visitor(*p);
		}
	}
}

void PainterComposite::setToolPainter(std::unique_ptr<lucca::Painter> painter) {
	removePaintersAt(_toolPaintingOrder);

	if (painter) {
		addSubPainterAt(std::move(painter), _toolPaintingOrder);
	}
}

void PainterComposite::setToolPainterOrder(SizeType order) {
	removePaintersAt(_toolPaintingOrder);
	_toolPaintingOrder = order;
}

void PainterComposite::initialize(RenderTarget* target) {
	visit(
		[target](Painter& painter) {
			painter.initialize(target);
		}
	);
}

void PainterComposite::dispose() {
	visit(
		[](Painter& painter) {
			painter.dispose();
		}
	);
}

bool PainterComposite::paint(const Camera& camera, const std::function<FilterAction(const Painter& painter, SizeType paintingOrder)>& filter) {
	_paintAfterEverything.clear();

	auto success = true;

	for (const auto& order : _painters) {
		_paintAfterOthersInSameOrder.clear();

		for (const auto& p : order.second) {
			const auto action = filter(*p, order.first);

			switch (action) {
			case FilterAction::Paint: success &= p->paint(camera); break;
			case FilterAction::PaintAfterOthersInSameOrder: _paintAfterOthersInSameOrder.push_back(p.get()); break;
			case FilterAction::PaintAfterEverything: _paintAfterEverything.push_back(p.get()); break;
			case FilterAction::DoNotPaint: break;
			default: break;
			}
		}

		for (auto& p : _paintAfterOthersInSameOrder) {
			success &= p->paint(camera);
		}
	}

	for (auto& p : _paintAfterEverything) {
		success &= p->paint(camera);
	}

	return success;
}

bool PainterComposite::paint(const Camera& camera) {
	auto success = true;

	visit(
		[&success, &camera](Painter& painter) {
			success &= painter.paint(camera);
		}
	);

	return success;
}

void PainterComposite::resize(SizeType width, SizeType height) {
	visit(
		[width, height](Painter& painter) {
			painter.resize(width, height);
		}
	);
}

bool PainterComposite::compareRenderTarget(const RenderTarget* target) const {
	for (const auto& order : _painters) {
		for (const auto& p : order.second) {
			if (!p->compareRenderTarget(target)) {
				return false;
			}
		}
	}

	return true;
}

bool PainterComposite::hasCurrentRenderingContext() const {
	for (const auto& order : _painters) {
		for (const auto& p : order.second) {
			if (!p->hasCurrentRenderingContext()) {
				return false;
			}
		}
	}

	return true;
}

ayla::AxisAlignedBox PainterComposite::getAABB() const {
	ayla::AxisAlignedBox aabb;
		
	visit(
		[&aabb](Painter& painter) {
			aabb = ayla::AxisAlignedBox::merge(aabb, painter.getAABB());
		}
	);
		
	return aabb;
}

std::unique_ptr<Painter> PainterComposite::clone() const {
	auto copy = std::make_unique<PainterComposite>();
	copy->_toolPaintingOrder = _toolPaintingOrder;
		
	for (const auto& order : _painters) {
		for (const auto& p : order.second) {
			copy->_painters[order.first].push_back(
				p->clone()
			);
		}
	}
		
	return std::move(copy);
}

void PainterComposite::addSubPainter(std::unique_ptr<Painter> subPainter) {
	setToolPainter(std::move(subPainter)); // warning: maybe this is not expected by the user
}

}