#ifndef LUCCA_MULTI_VIEW_DOCUMENT_HPP
#define LUCCA_MULTI_VIEW_DOCUMENT_HPP

#include "lucca/config.hpp"

#include "lucca/document/document.hpp"
#include "lucca/view/view_signals.hpp"

namespace lucca {

/**
 * @author O Programador
 */
class LUCCA_API MultiViewDocument : public Document {
public:
	virtual ~MultiViewDocument() = default;

	virtual void visitSubViews(const std::function<void(View&)>&) const = 0;

	virtual SizeType getNumberOfSubViews() const = 0;

	virtual View* getActiveSubView() const = 0;

	virtual void removeAllSubViews() = 0;

	virtual void postRedisplayAll() const = 0;

	virtual boost::signals2::connection connectToSubViewActivatedSignal(const SubViewActivatedSignalListener& listener) = 0;
};

}

#endif // LUCCA_MULTI_VIEW_DOCUMENT_HPP