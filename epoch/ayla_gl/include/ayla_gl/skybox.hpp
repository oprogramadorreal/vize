#ifndef AYLA_GL_SKYBOX_HPP
#define AYLA_GL_SKYBOX_HPP

#include "ayla_gl/config.hpp"

#include <boost/noncopyable.hpp>

namespace ayla_gl {

class TextureCubeMap;

/**
 * @author O Programador
 */
class AYLA_GL_API Skybox final : boost::noncopyable {
public:
	Skybox(std::unique_ptr<TextureCubeMap> texture);
	~Skybox();

	void draw(const glm::mat4& view, const glm::mat4& projection) const;

private:
	std::unique_ptr<TextureCubeMap> _texture;

	struct Private;
	std::unique_ptr<Private> _pimpl;

	class Shader;
	std::unique_ptr<Shader> _shader;
};

}

#endif // AYLA_GL_SKYBOX_HPP