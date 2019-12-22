#include "vize/factory/main_window_factory.hpp"

#include "vize/ui/main_window.hpp"

#include "vize/document/volume_document.hpp"

#include "vize/tool/descriptors.hpp"
#include "vize/tool/show_about_tool.hpp"
#include "vize/tool/open_document_tool.hpp"
#include "vize/tool/save_as_document_tool.hpp"
#include "vize/tool/import_dicom_tool.hpp"
#include "vize/tool/import_tiff_tool.hpp"
#include "vize/tool/import_raw_tool.hpp"

#include <lucca_qt/make_qt_action_tool.hpp>

namespace vize {

std::unique_ptr<MainWindow> MainWindowFactory::create(std::unique_ptr<lucca::MultiViewDocument> document) const {
	auto mainWindow = std::make_unique<MainWindow>(std::move(document));

	if (const auto document = dynamic_cast<VolumeDocument*>(mainWindow->getDocument())) {
		mainWindow->addActionToFileMenu(lucca_qt::makeQtActionTool<OpenDocumentTool>(mainWindow.get(), *document, mainWindow.get()).release());
		mainWindow->addActionToFileMenu(lucca_qt::makeQtActionTool<SaveAsDocumentTool>(mainWindow.get(), *document, mainWindow.get()).release());
		mainWindow->addActionToImportMenu(lucca_qt::makeQtActionTool<ImportTiffTool>(mainWindow.get(), *document, mainWindow.get()).release());
		mainWindow->addActionToImportMenu(lucca_qt::makeQtActionTool<ImportDicomTool>(mainWindow.get(), *document, mainWindow.get()).release());
		mainWindow->addActionToImportMenu(lucca_qt::makeQtActionTool<ImportRawTool>(mainWindow.get(), *document, mainWindow.get()).release());
	}

	mainWindow->addActionToHelpMenu(lucca_qt::makeQtActionTool<ShowAboutTool>(mainWindow.get()).release());

	return mainWindow;
}

}