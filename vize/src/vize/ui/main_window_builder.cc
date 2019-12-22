#include "vize/ui/main_window_builder.hpp"
#include "vize/ui/main_window.hpp"

#include "vize/document/volume_document.hpp"

#include "vize/tool/descriptors.hpp"
#include "vize/tool/show_about_tool.hpp"
#include "vize/tool/open_document_tool.hpp"
#include "vize/tool/save_as_document_tool.hpp"
#include "vize/tool/import_dicom_tool.hpp"
#include "vize/tool/import_tiff_tool.hpp"

#include <lucca_qt/make_qt_action_tool.hpp>

namespace vize {

void MainWindowBuilder::build(MainWindow& mainWindow) const {
	if (const auto document = dynamic_cast<VolumeDocument*>(mainWindow.getDocument())) {
		mainWindow.addActionToFileMenu(lucca_qt::makeQtActionTool<OpenDocumentTool>(&mainWindow, *document, &mainWindow).release());
		mainWindow.addActionToFileMenu(lucca_qt::makeQtActionTool<SaveAsDocumentTool>(&mainWindow, *document, &mainWindow).release());
		mainWindow.addActionToImportMenu(lucca_qt::makeQtActionTool<ImportTiffTool>(&mainWindow, *document, &mainWindow).release());
		mainWindow.addActionToImportMenu(lucca_qt::makeQtActionTool<ImportDicomTool>(&mainWindow, *document, &mainWindow).release());
	}

	mainWindow.addActionToHelpMenu(lucca_qt::makeQtActionTool<ShowAboutTool>(&mainWindow).release());
}

}