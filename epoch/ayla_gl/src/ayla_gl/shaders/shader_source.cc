#include "ayla_gl/shaders/shader_source.hpp"

#include <fstream>
#include <string>

namespace ayla_gl {

ShaderSource::ShaderSource() = default;

ShaderSource::ShaderSource(const std::string& sourceCode)
	: _sourceCode(sourceCode)
{ }

ShaderSource::ShaderSource(const std::string& sourceCode, const std::string& sourceFile)
	: _sourceCode(sourceCode), _sourceFile(sourceFile)
{ }

ShaderSource::~ShaderSource() = default;

ShaderSource ShaderSource::fromFile(const std::string& sourceFile) {
	std::ifstream stream(sourceFile, std::ios::in);
	auto sourceCode = std::string("");

	if (!stream.is_open()) {
		AYLA_DEBUG_MESSAGE(std::string("Impossible to open ") + sourceFile);
	} else {
		auto line = std::string("");

		while (std::getline(stream, line)) {
			sourceCode += "\n" + line;
		}

		stream.close();
	}

	return ShaderSource(sourceCode, sourceFile);
}

ShaderSource ShaderSource::fromCode(const std::string& sourceCode) {
	return ShaderSource(sourceCode);
}

std::string ShaderSource::getSourceCode() const {
	return _sourceCode;
}

std::string ShaderSource::getSourceFile() const {
	return _sourceFile;
}

bool ShaderSource::hasSourceCode() const {
	return !_sourceCode.empty();
}

bool ShaderSource::hasSourceFile() const {
	return !_sourceFile.empty();
}

}