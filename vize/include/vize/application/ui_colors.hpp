#ifndef VIZE_UI_COLORS_HPP
#define VIZE_UI_COLORS_HPP

#include "vize/config.hpp"

namespace vize {

/**
 * @author O Programador
 */
class UiColors final {
public:
	static glm::vec3 getTransferFunctionBarColor();
	static glm::vec3 getRayCastringBackgroundColor();
};

}

#endif // VIZE_UI_COLORS_HPP