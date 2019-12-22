#include "lucca_qt/image/create_opengl_texture_2d.hpp"

#include "lucca_qt/image/create_image.hpp"

#include <ayla_gl/texture_2d.hpp>
#include <ayla/image.hpp>

namespace lucca_qt {

std::unique_ptr<ayla_gl::Texture2D> createOpenGLTexture2D(const std::string& imageFile, bool repeat) {
	const auto image = createImage(imageFile);

	if (!image) {
		return nullptr;
	}

	return std::make_unique<ayla_gl::Texture2D>(*image, repeat);
}

}