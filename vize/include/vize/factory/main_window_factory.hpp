#ifndef VIZE_MAIN_WINDOW_FACTORY_HPP
#define VIZE_MAIN_WINDOW_FACTORY_HPP

#include "vize/config.hpp"

namespace lucca {
	class MultiViewDocument;
}

namespace vize {

class MainWindow;

/**
 * @author O Programador
 */
class MainWindowFactory final {
public:
	std::unique_ptr<MainWindow> create(std::unique_ptr<lucca::MultiViewDocument> document) const;
};

}

#endif // VIZE_MAIN_WINDOW_FACTORY_HPP