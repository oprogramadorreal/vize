#ifndef VIZE_MAIN_WINDOW_BUILDER_HPP
#define VIZE_MAIN_WINDOW_BUILDER_HPP

#include "vize/config.hpp"

namespace vize {

class MainWindow;

/**
 * @author O Programador
 */
class MainWindowBuilder final {
public:
	void build(MainWindow& mainWindow) const;
};

}

#endif // VIZE_MAIN_WINDOW_BUILDER_HPP