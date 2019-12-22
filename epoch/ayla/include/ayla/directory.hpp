#ifndef AYLA_DIRECTORY_HPP
#define AYLA_DIRECTORY_HPP

#include "ayla/config.hpp"

#include <string>
#include <vector>

namespace ayla {

AYLA_API bool fileExists(const std::string& fileName);

AYLA_API bool isDirectory(const std::string& str);

/**
 * Try to find a directory called @param dirName.
 *
 * @return The relative path to @param directoryName directory or an empty string if this directory was not found.
 */
AYLA_API std::string findDirectory(const std::string& directoryName);

struct File final {
	std::string name;
	std::string extension;
};

AYLA_API std::vector<File> getFilesInDirectory(const std::string& directory);

AYLA_API std::vector<std::string> getFilesPathInDirectory(const std::string& directory);

}

#endif // AYLA_DIRECTORY_HPP