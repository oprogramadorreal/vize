#include "ayla_gl/shaders/default_shaders_directory.hpp"

#include <ayla/directory.hpp>

namespace ayla_gl {

std::string getDefaultShadersDirectoryPath() {
	static std::string defaultShadersDirectoryPath;
	
	if (defaultShadersDirectoryPath.empty()) {
		defaultShadersDirectoryPath = ayla::findDirectory("epoch/ayla_gl/shaders");
		AYLA_DEBUG_MESSAGE("ayla::getDefaultShadersDirectoryPath() == " + defaultShadersDirectoryPath);
	}

	assert(!defaultShadersDirectoryPath.empty());

	return defaultShadersDirectoryPath;
}

}