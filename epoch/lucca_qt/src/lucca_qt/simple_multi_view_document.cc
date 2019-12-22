#include "lucca_qt/simple_multi_view_document.hpp"

#include <lucca/view/view_composite.hpp>

#include <QTimer>

namespace lucca_qt {

namespace {

/**
 * @author O Programador
 */
class SubWindowTitleGenerator final {
public:
	SubWindowTitleGenerator(const lucca::MultiViewDocument& document)
		: _document(document)
	{ }

	std::string operator()() const {
		auto windowTitle = _document.getDocumentName();
		const auto numberOfSubWindows = SizeType(_document.getNumberOfSubViews());
		
		if (numberOfSubWindows > SizeType(1)) {
			windowTitle += " [" + std::to_string(numberOfSubWindows-1) + "]";
		}
		
		return windowTitle;
	}

private:
	const lucca::MultiViewDocument& _document;
};

}

SimpleMultiViewDocument::SimpleMultiViewDocument(std::unique_ptr<lucca::ViewComposite> viewComposite)
	: _viewComposite(std::move(viewComposite))
{
	assert(_viewComposite);
	_viewComposite->setSubWindowTitleGenerator(SubWindowTitleGenerator(*this));
}

SimpleMultiViewDocument::~SimpleMultiViewDocument() = default;

lucca::View* SimpleMultiViewDocument::getView() const {
	return _viewComposite ? _viewComposite.get() : nullptr;
}

void SimpleMultiViewDocument::postRedisplay() const {
	if (const auto view = getView()) {
		view->postRedisplay();
	}
}

void SimpleMultiViewDocument::startContinuousPostRedisplay() {
	if (_redisplayTimer) {
		return;
	}

	_redisplayTimer = std::make_unique<QTimer>();
	QObject::connect(_redisplayTimer.get(), &QTimer::timeout, [this]() { postRedisplayAll(); });
	_redisplayTimer->start();
}

void SimpleMultiViewDocument::stopContinuousPostRedisplay() {
	if (!_redisplayTimer) {
		return;
	}

	_redisplayTimer->stop();
	QObject::disconnect(_redisplayTimer.get(), &QTimer::timeout, 0, 0);
	_redisplayTimer.reset();

	postRedisplayAll();
}

void SimpleMultiViewDocument::visitSubViews(const std::function<void(lucca::View&)>& visitor) const {
	if (_viewComposite) {
		_viewComposite->visitSubViews(visitor);
	}
}

SizeType SimpleMultiViewDocument::getNumberOfSubViews() const {
	return _viewComposite ? _viewComposite->getNumberOfSubViews() : SizeType(0);
}

lucca::View* SimpleMultiViewDocument::getActiveSubView() const {
	return _viewComposite ? _viewComposite->getActiveSubView() : nullptr;
}

void SimpleMultiViewDocument::removeAllSubViews() {
	if (_viewComposite) {
		_viewComposite->removeAllSubViews();
	}
}

void SimpleMultiViewDocument::postRedisplayAll() const {
	visitSubViews(
		[](lucca::View& subView) { subView.postRedisplay(); }
	);
}

boost::signals2::connection SimpleMultiViewDocument::connectToSubViewActivatedSignal(const lucca::SubViewActivatedSignalListener& listener) {
	if (!_viewComposite) {
		return boost::signals2::connection();
	}

	return _viewComposite->connectToSubViewActivatedSignal(listener);
}

bool SimpleMultiViewDocument::_addSubView(std::unique_ptr<lucca::View> subView) const {
	return _viewComposite ? _viewComposite->addSubView(std::move(subView)) : false;
}

}