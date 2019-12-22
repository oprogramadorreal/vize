#include "vize/shader/default_shaders_directory.hpp"

#include <ayla/directory.hpp>

namespace vize {

std::string getDefaultShadersDirectoryPath() {
	static std::string defaultShadersDirectoryPath;

	if (defaultShadersDirectoryPath.empty()) {
		defaultShadersDirectoryPath = ayla::findDirectory("vize/resources/shaders");
		AYLA_DEBUG_MESSAGE("vize::getDefaultShadersDirectoryPath() == " + defaultShadersDirectoryPath);
	}

	assert(!defaultShadersDirectoryPath.empty());

	return defaultShadersDirectoryPath;
}

}