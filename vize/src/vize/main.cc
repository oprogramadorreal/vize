#include "vize/application/application.hpp"

#include "vize/application/import_volume_launcher.hpp"
#include "vize/application/open_volume_launcher.hpp"

#include "vize/factory/document_factory.hpp"
#include "vize/factory/main_window_factory.hpp"
#include "vize/factory/volume_painter_factory.hpp"
#include "vize/factory/sub_view_factory.hpp"

#include "vize/opengl/gl_volume_texture_cache.hpp"

#include "vize/ui/main_window.hpp"
#include "vize/document/volume_document.hpp"

int main(int argc, char** argv) {
	vize::Application app(argc, argv);

	auto document = vize::DocumentFactory().create("Empty Document");

	if (!document) {
		return 1;
	}

	auto mainWindow = vize::MainWindowFactory().create(std::move(document));

	if (!mainWindow) {
		return 1;
	}

	auto volumePainterFactory = std::make_unique<vize::VolumePainterFactory>();
	auto subViewFactory = std::make_unique<vize::SubViewFactory>();

	std::unique_ptr<vize::ApplicationLauncher> launcher;

	if (const auto document = dynamic_cast<vize::VolumeDocument*>(mainWindow->getDocument())) {
		document->setVolumeTextureCache(std::make_shared<vize::GLVolumeTextureCache>());

		launcher = std::make_unique<vize::ImportCTHEAD>(*document, std::move(volumePainterFactory), std::move(subViewFactory));
		//launcher = std::make_unique<vize::ImportMANIX>(*document, std::move(volumePainterFactory), std::move(subViewFactory));
		//launcher = std::make_unique<vize::OpenMANIX>(*document);
	}

	return app.run(std::move(mainWindow), std::move(launcher));
}