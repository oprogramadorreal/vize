#include "lucca_qt/read_shader_resource.hpp"

#include <QFile>

namespace lucca_qt {

ayla_gl::ShaderSource readShaderResource(const std::string& resourceFile) {
	auto shaderResource = ayla_gl::ShaderSource();
	QFile file(QString::fromStdString(resourceFile));

	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		auto sourceCode = std::string("");

		while (!file.atEnd()) {
			sourceCode += file.readLine().toStdString();
		}

		file.close();
		shaderResource = ayla_gl::ShaderSource::fromCode(sourceCode);

	} else {
		AYLA_DEBUG_MESSAGE("WARNING: cannot open shader resource!");
	}

	return shaderResource;
}

}