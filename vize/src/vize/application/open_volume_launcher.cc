#include "vize/application/open_volume_launcher.hpp"

#include "vize/tool/open_document_tool.hpp"

#include <ayla/directory.hpp>

namespace vize {

OpenMANIX::OpenMANIX(VolumeDocument& document)
	: _document(document)
{ }

OpenMANIX::~OpenMANIX() = default;

void OpenMANIX::run() {
	const auto volumesDirectory = ayla::findDirectory("vize/volumes");

	if (!volumesDirectory.empty()) {
		const auto volumeFile = volumesDirectory + "manix.vize";

		if (ayla::fileExists(volumeFile)) {
			OpenDocumentTool(_document).open(volumeFile);
		}
	}
}

bool OpenMANIX::isRunning() const {
	return false;
}

void OpenMANIX::showProgressDialog(const std::string&) { }

}