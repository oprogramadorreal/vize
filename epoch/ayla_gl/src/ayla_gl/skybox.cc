#include <GL/glew.h>

#include "ayla_gl/skybox.hpp"
#include "ayla_gl/texture_cube_map.hpp"
#include "ayla_gl/shaders/shader_program.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <array>

namespace ayla_gl {

namespace {
	const std::array<GLfloat, 108> skyboxVertices = { 
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

	const auto vs_source = std::string(
		"#version 330 core\n"

		"layout(location = 0) in vec3 position;"
		"out vec3 texCoords;"

		"uniform mat4 P;"
		"uniform mat4 V;"

		"void main() {"
		"	vec4 pos = P * V * vec4(position, 1.0);"
		"	gl_Position = pos.xyww;"
		"	texCoords = position;"
		"}"
	);

	const auto fs_source = std::string(
		"#version 330 core\n"

		"in vec3 texCoords;"
		"out vec4 color;"

		"uniform samplerCube skybox;"

		"void main() {"
		"	color = texture(skybox, texCoords);"
		"}"
	);
}

/**
 * @author O Programador
 */
class Skybox::Shader final : public ShaderProgram {
public:
	Shader()
		: ShaderProgram(ShaderSource::fromCode(vs_source), ShaderSource::fromCode(fs_source))
	{
		_V = glGetUniformLocation(getId(), "V");
		_P = glGetUniformLocation(getId(), "P");
		_skybox = glGetUniformLocation(getId(), "skybox");
	}

	inline GLuint getUniform_V() const { return _V; }
	inline GLuint getUniform_P() const { return _P; }
	inline GLuint getUniform_skybox() const { return _skybox; }

private:
	GLuint _V = GLuint(0);
	GLuint _P = GLuint(0);
	GLuint _skybox = GLuint(0);
};

struct Skybox::Private final {
	GLuint vao = GLuint(0);
	GLuint vbo = GLuint(0);
};

Skybox::Skybox(std::unique_ptr<TextureCubeMap> texture)
	: _texture(std::move(texture)), _pimpl(std::make_unique<Private>()), _shader(std::make_unique<Shader>())
{
	glGenBuffers(1, &_pimpl->vbo);

	glGenVertexArrays(1, &_pimpl->vao);
	glBindVertexArray(_pimpl->vao);

	glBindBuffer(GL_ARRAY_BUFFER, _pimpl->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * skyboxVertices.size(), (GLvoid*)skyboxVertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindVertexArray(0);
}

Skybox::~Skybox() {
	glDeleteBuffers(1, &_pimpl->vbo);
	glDeleteVertexArrays(1, &_pimpl->vao);
}

void Skybox::draw(const glm::mat4& view, const glm::mat4& projection) const {
	glPushAttrib(GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LEQUAL);

	glUseProgram(_shader->getId());

	const auto viewMatrix = glm::mat4(glm::mat3(view));	// remove translation component
	glUniformMatrix4fv(_shader->getUniform_V(), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(_shader->getUniform_P(), 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(_pimpl->vao);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(_shader->getUniform_skybox(), 0);
	_texture->bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glUseProgram(0);

	glPopAttrib();
}

}