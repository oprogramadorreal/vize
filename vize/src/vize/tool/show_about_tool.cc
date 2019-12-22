#include "vize/tool/show_about_tool.hpp"

#include <QSplashScreen>

namespace vize {

ShowAboutTool::ShowAboutTool() = default;
ShowAboutTool::~ShowAboutTool() = default;

void ShowAboutTool::_activateImpl() {
	_splash = std::make_unique<QSplashScreen>(QPixmap(":resources/splash"));
	_splash->setWindowFlags(_splash->windowFlags() | Qt::WindowStaysOnTopHint);

	_splash->showMessage(
		QString("For more information, please visit:<br><a href='https://sites.google.com/site/ssmarco11/'>https://sites.google.com/site/ssmarco11</a><br><a href='https://www.youtube.com/oprogramadorreal'>https://www.youtube.com/oprogramadorreal</a>"),
		Qt::AlignRight
	);

	_splash->show();
}

}