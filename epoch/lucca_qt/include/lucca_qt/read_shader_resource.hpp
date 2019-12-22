#ifndef LUCCA_QT_READ_SHADER_RESOURCE_HPP
#define LUCCA_QT_READ_SHADER_RESOURCE_HPP

#include "lucca_qt/config.hpp"

#include <ayla_gl/shaders/shader_source.hpp>

namespace lucca_qt {

LUCCA_QT_API ayla_gl::ShaderSource readShaderResource(const std::string& resourceFile);

}

#endif // LUCCA_QT_READ_SHADER_RESOURCE_HPP