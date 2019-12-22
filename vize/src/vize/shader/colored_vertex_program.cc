#include <GL/glew.h>

#include "vize/shader/colored_vertex_program.hpp"

#include <lucca_qt/read_shader_resource.hpp>

namespace vize {

ColoredVertexProgram::ColoredVertexProgram()
	: ShaderProgram(
		lucca_qt::readShaderResource(":resources/shaders/colored_vertex.vs"),
		lucca_qt::readShaderResource(":resources/shaders/colored_vertex.fs")
	)
{
	_M = glGetUniformLocation(getId(), "M");
	_V = glGetUniformLocation(getId(), "V");
	_P = glGetUniformLocation(getId(), "P");
}

}