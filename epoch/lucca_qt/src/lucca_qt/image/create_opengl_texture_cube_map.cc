#include "lucca_qt/image/create_opengl_texture_cube_map.hpp"

#include "lucca_qt/image/create_image_cube_map.hpp"

#include <ayla_gl/texture_cube_map.hpp>
#include <ayla/image_cube_map.hpp>

namespace lucca_qt {

std::unique_ptr<ayla_gl::TextureCubeMap> createOpenGLTextureCubeMap(
	const std::string& imageFilePositiveX,
	const std::string& imageFileNegativeX,
	const std::string& imageFilePositiveY,
	const std::string& imageFileNegativeY,
	const std::string& imageFilePositiveZ,
	const std::string& imageFileNegativeZ
) {
	const auto image = createImageCubeMap(
		imageFilePositiveX, imageFileNegativeX,
		imageFilePositiveY, imageFileNegativeY,
		imageFilePositiveZ, imageFileNegativeZ
	);

	if (!image) {
		return nullptr;
	}

	return std::make_unique<ayla_gl::TextureCubeMap>(*image);
}

}