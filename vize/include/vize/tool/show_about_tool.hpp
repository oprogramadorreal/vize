#ifndef VIZE_SHOW_ABOUT_TOOL_HPP
#define VIZE_SHOW_ABOUT_TOOL_HPP

#include "vize/config.hpp"

#include <lucca/tool/tool_impl.hpp>

class QSplashScreen;

namespace vize {

/**
 * @author O Programador
 */
class ShowAboutTool final : public lucca::ToolImpl {
public:
	ShowAboutTool();
	virtual ~ShowAboutTool();

private:
	virtual void _activateImpl() final override;

private:
	std::unique_ptr<QSplashScreen> _splash;
};

}

#endif // VIZE_SHOW_ABOUT_TOOL_HPP