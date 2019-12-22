#include "vize/ui/main_window.hpp"
#include "ui_main_window.h"

#include <lucca/document/document.hpp>
#include <lucca_qt/qt_action_tool.hpp>
#include <lucca_qt/qt_mdi_area_view.hpp>

namespace vize {

MainWindow::MainWindow(std::unique_ptr<lucca::Document> document)
	: _ui(std::make_unique<Ui::MainWindow>()), _document(std::move(document))
{
	_ui->setupUi(this);
	assert(_document);

	if (auto mdiView = dynamic_cast<lucca_qt::QtMdiAreaView*>(_document->getView())) {
		setCentralWidget(mdiView); // QMainWindow takes ownership of the widget pointer and deletes it at the appropriate time.
		//MainWindow::connect(mdiView, &QMdiArea::subWindowActivated, this, &MainWindow::_subWindowActivated);
	}
}

MainWindow::~MainWindow() { }

lucca::Document* MainWindow::getDocument() const {
	return _document.get();
}

void MainWindow::addActionToFileMenu(QAction* action) {
	_ui->menuFile->addAction(action);
}

void MainWindow::addActionToHelpMenu(QAction* action) {
	_ui->menuHelp->addAction(action);
}

void MainWindow::addActionToImportMenu(QAction* action) {
	if (!_importMenu) {
		_importMenu = std::make_unique<QMenu>(_ui->menuFile).release();
		_importMenu->setTitle("Import");
		_ui->menuFile->addMenu(_importMenu);
	}

	_importMenu->addAction(action);
}

}