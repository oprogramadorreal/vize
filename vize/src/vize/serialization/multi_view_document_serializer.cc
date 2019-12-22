#include "vize/serialization/multi_view_document_serializer.hpp"

#include "vize/painter/volume_painter.hpp"
#include "vize/document/sub_view.hpp"
#include "vize/document/sub_view_state.hpp"
#include "vize/factory/sub_view_factory.hpp"

#include <lucca/document/multi_view_document.hpp>
#include <lucca/view/view.hpp>
#include <lucca/camera.hpp>
#include <lucca_qt/qt_opengl_view.hpp>

#include <ayla/serialization/boost/explicit_instantiation_macros.hpp>

namespace boost {
namespace serialization {

template<class Archive>
void save(Archive& ar, const lucca::MultiViewDocument& document, const unsigned int version) {
	auto allPainters = std::vector<std::pair<vize::VolumePainter*, lucca::Camera*> >();

	// collect all cameras and painters
	document.visitSubViews(
		[&allPainters](lucca::View& view) {
			if (const auto camera = view.getDefaultCamera()) {
				if (const auto painter = dynamic_cast<vize::VolumePainter*>(view.getPainter())) {
					allPainters.push_back(std::make_pair(painter, camera));
				}
			}
		}
	);

	const auto numberOfPainters = vize::SizeType(allPainters.size());
	ar & make_nvp("numberOfPainters", numberOfPainters);

	// save each painter and its cameras
	for (auto i = vize::SizeType(0); i < numberOfPainters; ++i) {
		auto painter = allPainters[i].first;
		auto camera = allPainters[i].second;
		assert(painter);
		assert(camera);

		// serialize painter
		ar & make_nvp("painter", painter);

		// serialize camera // TODO: save multiple cameras
		ar & make_nvp("camera", *camera);
	}
}

template<class Archive>
void load(Archive& ar, lucca::MultiViewDocument& document, const unsigned int version) {
	auto numberOfPainters = vize::SizeType(0);
	ar & make_nvp("numberOfPainters", numberOfPainters);

	// load each painter and its cameras (may be slow)
	for (auto i = vize::SizeType(0); i < numberOfPainters; ++i) {
		vize::VolumePainter* painter = nullptr;
		ar & make_nvp("painter", painter);

		if (painter) {
			auto widgetView = std::make_unique<lucca_qt::QtOpenGLView>(std::unique_ptr<lucca::Painter>(painter));

			if (auto subView = vize::SubViewFactory().create(std::move(widgetView))) {
				if (auto defaultCamera = subView->getDefaultCamera()) { // TODO: load multiple cameras
					ar & make_nvp("camera", *defaultCamera);
				}

				subView->setState(vize::SubViewState::fromPainter(*painter));

				document.addView(std::move(subView));
			}
		}
	}
}

EXPLICIT_INSTANTIATION_SAVE_FUNC(lucca::MultiViewDocument);
EXPLICIT_INSTANTIATION_LOAD_FUNC(lucca::MultiViewDocument);

}
}