#include "vize/factory/volume_painter_factory.hpp"

#include "vize/painter/volume_painter.hpp"

namespace vize {

std::unique_ptr<lucca::Painter> VolumePainterFactory::create(Volume& volume) {
	return std::make_unique<VolumePainter>(&volume);
}

}