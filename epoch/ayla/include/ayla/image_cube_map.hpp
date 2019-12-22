#ifndef AYLA_IMAGE_CUBE_MAP_HPP
#define AYLA_IMAGE_CUBE_MAP_HPP

#include "ayla/config.hpp"

#include "ayla/serialization/image_cube_map_serializer.hpp"

#include <array>

namespace ayla {

class Image;

/**
 * @see ayla_gl::Image
 * @see ayla_gl::TextureCubeMap
 *
 * @author O Programador
 */
class AYLA_API ImageCubeMap final {
private:
	ImageCubeMap();

public:
	ImageCubeMap(
		std::unique_ptr<Image> positiveX,
		std::unique_ptr<Image> negativeX,
		std::unique_ptr<Image> positiveY,
		std::unique_ptr<Image> negativeY,
		std::unique_ptr<Image> positiveZ,
		std::unique_ptr<Image> negativeZ
	);

	~ImageCubeMap();

	enum class Direction {
		PositiveX = 0,
		NegativeX = 1,
		PositiveY = 2,
		NegativeY = 3,
		PositiveZ = 4,
		NegativeZ = 5
	};

	const Image& getImage(Direction direction) const;

private:
	std::array<std::unique_ptr<Image>, 6u> _images;

	template <class Archive> friend void boost::serialization::serialize(Archive&, ayla::ImageCubeMap&, const unsigned int);
	template <class Archive> friend void boost::serialization::load_construct_data(Archive&, ayla::ImageCubeMap*, const unsigned int);
};

}

#endif // AYLA_IMAGE_CUBE_MAP_HPP