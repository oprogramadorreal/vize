#ifndef VIZE_APPLICATION_LAUNCHER_HPP
#define VIZE_APPLICATION_LAUNCHER_HPP

#include "vize/config.hpp"

#include <string>

namespace vize {

/**
 * @see vize::Application
 *
 * @author O Programador
 */
class ApplicationLauncher {
public:
	virtual ~ApplicationLauncher() = default;
	virtual void run() = 0;
	virtual bool isRunning() const = 0;
	virtual void showProgressDialog(const std::string& progressDialogTitle) = 0;
};

}

#endif // VIZE_APPLICATION_LAUNCHER_HPP