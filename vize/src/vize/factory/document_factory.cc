#include "vize/factory/document_factory.hpp"

#include "vize/document/volume_document.hpp"

#include <lucca_qt/qt_mdi_area_view.hpp>

namespace vize {

std::unique_ptr<lucca::MultiViewDocument> DocumentFactory::create(const std::string& documentName) const {
	auto view = std::make_unique<lucca_qt::QtMdiAreaView>();

	auto document = std::make_unique<VolumeDocument>(
		documentName, std::move(view)
	);

	return std::move(document);
}

}