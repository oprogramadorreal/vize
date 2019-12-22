#ifndef VIZE_OPEN_VOLUME_LAUNCHER_HPP
#define VIZE_OPEN_VOLUME_LAUNCHER_HPP

#include "vize/config.hpp"

#include "vize/application/application_launcher.hpp"

namespace vize {

class VolumeDocument;

/**
 * @author O Programador
 */
class OpenMANIX final : public ApplicationLauncher {
public:
	OpenMANIX(VolumeDocument& document);
	virtual ~OpenMANIX();

private:
	virtual void run() final override;
	virtual bool isRunning() const final override;
	virtual void showProgressDialog(const std::string& progressDialogTitle) final override;

private:
	VolumeDocument& _document;
};

}

#endif // VIZE_OPEN_VOLUME_LAUNCHER_HPP