#include <GL/glew.h>

#include "ayla_gl/renderers/text_renderer.hpp"
#include "ayla_gl/shaders/create_shader_program.hpp"
#include "ayla_gl/shaders/shader_source.hpp"
#include "ayla_gl/load_texture.hpp"

#include <vector>

namespace ayla_gl {

namespace {
	const auto vs_source =
		"#version 330 core\n"

		"layout(location = 0) in vec2 vertexPosition_homoneneousspace;"
		"layout(location = 1) in vec2 vertexUV;"
		"out vec2 UV;"

		"void main() {"
		"	gl_Position = vec4(vertexPosition_homoneneousspace, 0, 1);"
		"	UV = vertexUV;"
		"}";

	const auto fs_source =
		"#version 330 core\n"

		"in vec2 UV;"
		"out vec4 color;"
		"uniform sampler2D myTextureSampler;"

		"void main() {"
		"	color = texture2D(myTextureSampler, UV);"
		"}";
}

TextRenderer::TextRenderer(const std::string& fontTextureFile) {
	_texture = loadDDS(fontTextureFile);

	_program = createShaderProgram(ShaderSource::fromCode(std::string(vs_source)), ShaderSource::fromCode(std::string(fs_source)));
	_uniform = glGetUniformLocation(_program, "myTextureSampler");

	glGenBuffers(1, &_vertexBuffer);
	glGenBuffers(1, &_uvBuffer);

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, _uvBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindVertexArray(0);
}

TextRenderer::~TextRenderer() {
	glDeleteVertexArrays(1, &_vao);

	glDeleteBuffers(1, &_vertexBuffer);
	glDeleteBuffers(1, &_uvBuffer);

	glDeleteTextures(1, &_texture);

	glDeleteProgram(_program);
}

void TextRenderer::drawText2D(const std::string& text, float x, float y, float size) const {
	const SizeType length = SizeType(text.length());

	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> UVs;

	for (SizeType i = 0u; i < length; ++i) {
		const glm::vec2 vertex_up_left = glm::vec2(x + i*size, y + size);
		const glm::vec2 vertex_up_right = glm::vec2(x + i*size + size, y + size);
		const glm::vec2 vertex_down_right = glm::vec2(x + i*size + size, y);
		const glm::vec2 vertex_down_left = glm::vec2(x + i*size, y);

		vertices.push_back(vertex_up_left);
		vertices.push_back(vertex_down_left);
		vertices.push_back(vertex_up_right);

		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_left);

		const char character = text[i];
		const float uv_x = (character % 16) / 16.0f;
		const float uv_y = (character / 16) / 16.0f;

		const glm::vec2 uv_up_left = glm::vec2(uv_x, uv_y);
		const glm::vec2 uv_up_right = glm::vec2(uv_x + 1.0f / 16.0f, uv_y);
		const glm::vec2 uv_down_right = glm::vec2(uv_x + 1.0f / 16.0f, (uv_y + 1.0f / 16.0f));
		const glm::vec2 uv_down_left = glm::vec2(uv_x, (uv_y + 1.0f / 16.0f));

		UVs.push_back(uv_up_left);
		UVs.push_back(uv_down_left);
		UVs.push_back(uv_up_right);

		UVs.push_back(uv_down_right);
		UVs.push_back(uv_up_right);
		UVs.push_back(uv_down_left);
	}

	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, _uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);

	glUseProgram(_program);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glUniform1i(_uniform, 0); // set our "myTextureSampler" sampler to user Texture Unit 0

	glBindVertexArray(_vao);
	glDrawArrays(GL_TRIANGLES, 0, GLsizei(vertices.size()));
	glBindVertexArray(0);

	glDisable(GL_BLEND);
}

}