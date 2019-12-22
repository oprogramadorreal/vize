#ifndef VIZE_APPLICATION_HPP
#define VIZE_APPLICATION_HPP

#include "vize/config.hpp"

#include <QApplication>
#include <QSplashScreen>

namespace vize {

class MainWindow;
class ApplicationLauncher;

/**
 * @author O Programador
 */
class Application final {
public:
	Application(int argc, char** argv);
	~Application();

	int run(std::unique_ptr<MainWindow> mainWindow, std::unique_ptr<ApplicationLauncher> launcher = nullptr);

private:
	void _showMainWindow();
	bool _checkGraphicsConstraints();
	
private:
	QApplication _qtApp;
	QSplashScreen _qtSplash;
	std::unique_ptr<MainWindow> _mainWindow;
	std::unique_ptr<ApplicationLauncher> _launcher;
};

}

#endif // VIZE_APPLICATION_HPP