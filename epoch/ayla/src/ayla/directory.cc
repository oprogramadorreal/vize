#include "ayla/directory.hpp"

#include <array>

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp> 

namespace ayla {

bool fileExists(const std::string& fileName) {
	return boost::filesystem::exists(fileName);
}

bool isDirectory(const std::string& str) {
	return boost::filesystem::is_directory(str);
}

std::string findDirectory(const std::string& directoryName) {
	const std::array<std::string, 10> options = {
		"./" + directoryName + "/",
		"../" + directoryName + "/",
		"../../" + directoryName + "/",
		"../../../" + directoryName + "/",
		"../../../../" + directoryName + "/",
		"../../../../../" + directoryName + "/",
		"../../../../../../" + directoryName + "/",
		"../../../../../../../" + directoryName + "/",
		"../../../../../../../../" + directoryName + "/",
		"../../../../../../../../../" + directoryName + "/"
	};

	for (auto it = options.begin(); it != options.end(); ++it) {
		if (isDirectory(*it)) {
			return *it;
		}
	}

	return std::string();
}

std::vector<File> getFilesInDirectory(const std::string& directory) {
	std::vector<File> result;

	const boost::filesystem::path targetDir(directory);
	boost::filesystem::directory_iterator it(targetDir), eod;

	BOOST_FOREACH(boost::filesystem::path const &p, std::make_pair(it, eod)) {
		if (is_regular_file(p)) {
			File f;
			f.name = p.filename().string();
			f.extension = p.extension().string();
			result.push_back(f);
		}
	}

	return result;
}

std::vector<std::string> getFilesPathInDirectory(const std::string& directory) {
	const auto files = getFilesInDirectory(directory);

	std::vector<std::string> filesPathes;

	for (auto it = files.begin(); it != files.end(); ++it) {
		filesPathes.push_back(directory + it->name);
	}

	return filesPathes;
}

}