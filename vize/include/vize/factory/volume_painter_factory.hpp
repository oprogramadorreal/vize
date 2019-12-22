#ifndef VIZE_VOLUME_PAINTER_FACTORY_HPP
#define VIZE_VOLUME_PAINTER_FACTORY_HPP

#include "vize/config.hpp"

namespace lucca {
	class Painter;
}

namespace vize {

class Volume;

/**
 * @author O Programador
 */
class VolumePainterFactory final {
public:
	std::unique_ptr<lucca::Painter> create(Volume& volume);
};

}

#endif // VIZE_VOLUME_PAINTER_FACTORY_HPP