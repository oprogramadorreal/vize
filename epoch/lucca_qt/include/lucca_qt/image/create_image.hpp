#ifndef LUCCA_QT_CREATE_IMAGE_HPP
#define LUCCA_QT_CREATE_IMAGE_HPP

#include "lucca_qt/config.hpp"

#include <string>

namespace ayla {
	class Image;
}

namespace lucca_qt {

/**
 * @see ayla::Image
 */
LUCCA_QT_API std::unique_ptr<ayla::Image> createImage(const std::string& imageFile);

}

#endif // LUCCA_QT_CREATE_IMAGE_HPP