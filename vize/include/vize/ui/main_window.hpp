#ifndef VIZE_MAIN_WINDOW_HPP
#define VIZE_MAIN_WINDOW_HPP

#include "vize/config.hpp"

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

namespace lucca {
	class Document;
}

namespace vize {

/**
 * @author O Programador
 */
class MainWindow final : public QMainWindow {
public:
	explicit MainWindow(std::unique_ptr<lucca::Document> document);
	virtual ~MainWindow();

public:
	lucca::Document* getDocument() const;

	void addActionToFileMenu(QAction* action);
	void addActionToHelpMenu(QAction* action);
	void addActionToImportMenu(QAction* action);

private:
	const std::unique_ptr<Ui::MainWindow> _ui;
	const std::unique_ptr<lucca::Document> _document;
	QMenu* _importMenu = nullptr;
	
	class MainWindowBuilder;
};

}

#endif // VIZE_MAIN_WINDOW_HPP