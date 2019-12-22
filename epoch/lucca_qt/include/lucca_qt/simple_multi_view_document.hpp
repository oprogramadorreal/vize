#ifndef LUCCA_QT_SIMPLE_MULTI_VIEW_DOCUMENT_HPP
#define LUCCA_QT_SIMPLE_MULTI_VIEW_DOCUMENT_HPP

#include "lucca_qt/config.hpp"

#include <lucca/document/multi_view_document.hpp>

class QTimer;

namespace lucca {
	class ViewComposite;
}

namespace lucca_qt {

/**
 * @author O Programador
 */
class LUCCA_QT_API SimpleMultiViewDocument : public lucca::MultiViewDocument {
public:
	SimpleMultiViewDocument(std::unique_ptr<lucca::ViewComposite> viewComposite);
	virtual ~SimpleMultiViewDocument() = 0;

public: // partial lucca::Document interface
	virtual lucca::View* getView() const final override;
	virtual void postRedisplay() const final override;
	virtual void startContinuousPostRedisplay() final override;
	virtual void stopContinuousPostRedisplay() final override;

public: // lucca::MultiViewDocument interface
	virtual void visitSubViews(const std::function<void(lucca::View&)>&) const final override;
	virtual SizeType getNumberOfSubViews() const final override;
	virtual lucca::View* getActiveSubView() const final override;
	virtual void removeAllSubViews() final override;
	virtual void postRedisplayAll() const final override;
	virtual boost::signals2::connection connectToSubViewActivatedSignal(const lucca::SubViewActivatedSignalListener& listener) final override;

protected:
	bool _addSubView(std::unique_ptr<lucca::View> subView) const;

private:
	const std::unique_ptr<lucca::ViewComposite> _viewComposite;
	std::unique_ptr<QTimer> _redisplayTimer;
};

}

#endif // LUCCA_QT_SIMPLE_MULTI_VIEW_DOCUMENT_HPP