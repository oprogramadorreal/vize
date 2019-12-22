#include "lucca_qt/qt_mdi_area_view.hpp"

#include "lucca_qt/qt_mdi_sub_window_view.hpp"

namespace lucca_qt {

QtMdiAreaView::QtMdiAreaView() {
	setAttribute(Qt::WA_DeleteOnClose);
	connect(this, &QMdiArea::subWindowActivated, this, &QtMdiAreaView::_subWindowActivated);
}

QtMdiAreaView::~QtMdiAreaView() = default;

bool QtMdiAreaView::addSubView(std::unique_ptr<View> view) {
	const auto viewWidget = dynamic_cast<QWidget*>(view.get());

	if (!viewWidget) {
		return false;
	}

	const bool oneWindow = subWindowList().size() == 0;

	view.release();
	auto subWindow = addSubWindow(viewWidget, Qt::SubWindow);

	const auto g = geometry();
	subWindow->setGeometry(QRect(0, 0, (g.width() / 2) * 0.9f, g.height() * 0.9f));
	subWindow->setWindowTitle(QString::fromStdString(_titleGenerator()));
	subWindow->setAttribute(Qt::WA_DeleteOnClose);

	if (oneWindow) {
		subWindow->showMaximized();
	} else {
		subWindow->show();
		tileSubWindows();
	}

	return true;
}

void QtMdiAreaView::removeAllSubViews() {
	// We have set "Qt::WA_DeleteOnClose" attribute.
	// So this must delete all mdi windows:
	closeAllSubWindows();
}

SizeType QtMdiAreaView::getNumberOfSubViews() const {
	return SizeType(subWindowList().size());
}

void QtMdiAreaView::visitSubViews(const std::function<void(lucca::View&)>& visitor) const {
	const auto subWindows = subWindowList();

	for (const auto& subWin : subWindows) {
		if (const auto view = dynamic_cast<View*>(subWin)) {
			visitor(*view);
		}
	}
}

lucca::View* QtMdiAreaView::getActiveSubView() const {
	return dynamic_cast<View*>(activeSubWindow());
}

void QtMdiAreaView::setSubWindowTitleGenerator(const SubWindowTitleGenerator& titleGenerator) {
	_titleGenerator = titleGenerator;
}

lucca::ViewComposite::SubWindowTitleGenerator QtMdiAreaView::getSubWindowTitleGenerator() const {
	return _titleGenerator;
}

boost::signals2::connection QtMdiAreaView::connectToSubViewActivatedSignal(const lucca::SubViewActivatedSignalListener& listener) {
	return _subViewActivated.connect(listener);
}

std::unique_ptr<lucca::View> QtMdiAreaView::clone() const {
	auto copy = std::make_unique<QtMdiAreaView>();
	copy->setSubWindowTitleGenerator(_titleGenerator);

	visitSubViews(
		[&copy](View& subView) {
			copy->addSubView(subView.clone());
		}
	);

	return std::move(copy);
}

void QtMdiAreaView::_subWindowActivated(QMdiSubWindow* window) {
	if (const auto subView = dynamic_cast<View*>(window)) {
		_subViewActivated(*subView);
	}
}

}