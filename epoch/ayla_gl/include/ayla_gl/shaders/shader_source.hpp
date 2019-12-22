#ifndef AYLA_GL_SHADER_SOURCE_HPP
#define AYLA_GL_SHADER_SOURCE_HPP

#include "ayla_gl/config.hpp"

#include <string>

namespace ayla_gl {

/**
 * @author O Programador
 */
class AYLA_GL_API ShaderSource final {
private:
	explicit ShaderSource(const std::string& sourceCode);
	explicit ShaderSource(const std::string& sourceCode, const std::string& sourceFile);
	
public:
	ShaderSource();
	~ShaderSource();

	static ShaderSource fromFile(const std::string& sourceFile);
	static ShaderSource fromCode(const std::string& sourceCode);

public:
	std::string getSourceCode() const;
	std::string getSourceFile() const;

	bool hasSourceCode() const;
	bool hasSourceFile() const;

private:
	std::string _sourceCode;
	std::string _sourceFile;
};

}

#endif // AYLA_GL_SHADER_SOURCE_HPP