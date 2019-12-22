#include "vize/application/application.hpp"

#include "vize/ui/main_window.hpp"
#include "vize/application/application_launcher.hpp"

#include <QTimer>
#include <QOpenGLContext>
#include <QMessageBox>

namespace vize {

Application::Application(int argc, char** argv)
	: _qtApp(argc, argv), _qtSplash(QPixmap(":resources/splash"))
{
	_qtApp.setWindowIcon(QIcon(":/resources/vize"));
	_qtApp.setApplicationName("Vize");
	_qtApp.setOrganizationName("Marco");

	_qtSplash.setWindowFlags(_qtSplash.windowFlags() | Qt::WindowStaysOnTopHint);
}

Application::~Application() = default;

int Application::run(std::unique_ptr<MainWindow> mainWindow, std::unique_ptr<ApplicationLauncher> launcher) {
	if (!_checkGraphicsConstraints()) {
		return 1;
	}

	_mainWindow = std::move(mainWindow);

	_qtSplash.show();
	_qtApp.processEvents();

	if (launcher) {
		_launcher = std::move(launcher);
		_launcher->run();
	}

#ifdef NDEBUG
	const auto splashTime = 1000;
	QTimer timer;
	timer.setInterval(splashTime);
	timer.setSingleShot(true);
	
	QObject::connect(&timer, &QTimer::timeout,
		[this]() {
			_showMainWindow();
		}
	);

	timer.start();
#else
	_showMainWindow();
#endif

	return _qtApp.exec();
}

void Application::_showMainWindow() {
	if (_mainWindow) {
		_qtSplash.finish(_mainWindow.get());
		_mainWindow->showMaximized();
	}

	if (_launcher && _launcher->isRunning()) {
		_launcher->showProgressDialog("Loading sample.");
	}
}

bool Application::_checkGraphicsConstraints() {
	const auto majorVersionRequired = 3;
	const auto minorVersionRequired = 3;

	QOpenGLContext context;
	auto majorVersion = int(0);
	auto minorVersion = int(0);

	if (context.create()) {
		const auto format = context.format();
		majorVersion = format.majorVersion();
		minorVersion = format.minorVersion();

		if (majorVersion > majorVersionRequired) {
			return true;
		}

		if ((majorVersion == majorVersionRequired) && (minorVersion >= minorVersionRequired)) {
			return true;
		}
	}

	QMessageBox errorMessage;
	errorMessage.setWindowTitle(QString("Error!"));
	auto message = QString("It seems that your graphics card is not compatible with VIZE.\n");
	message += QString().sprintf("Your OpenGL version is %d.%d.\n\n", majorVersion, minorVersion);
	message += QString("Please contact support for more information.");
	
	errorMessage.setText(message);
	errorMessage.exec();

	return false;
}

}