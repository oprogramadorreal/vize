#ifndef LUCCA_QT_MDI_AREA_VIEW_HPP
#define LUCCA_QT_MDI_AREA_VIEW_HPP

#include "lucca_qt/config.hpp"

#include <lucca/view/view_composite.hpp>

#include <QMdiArea>

namespace lucca_qt {

class QtMdiSubWindowView;

/**
 * Concrete abstraction of lucca::ViewComposite.
 *
 * @see lucca::ViewComposite
 *
 * @author O Programador
 */
class LUCCA_QT_API QtMdiAreaView final : public QMdiArea, public lucca::ViewComposite {
public:
	QtMdiAreaView();
	virtual ~QtMdiAreaView();

private: // lucca::ViewComposite interface
	virtual bool addSubView(std::unique_ptr<View> view) final override;

	virtual void removeAllSubViews() final override;

	virtual SizeType getNumberOfSubViews() const final override;

	virtual void visitSubViews(const std::function<void(lucca::View&)>& visitor) const final override;

	virtual View* getActiveSubView() const final override;

	virtual void setSubWindowTitleGenerator(const SubWindowTitleGenerator& titleGenerator) final override;

	virtual SubWindowTitleGenerator getSubWindowTitleGenerator() const final override;

	virtual boost::signals2::connection connectToSubViewActivatedSignal(const lucca::SubViewActivatedSignalListener& listener) final override;

private: // lucca::View interface
	std::unique_ptr<View> clone() const final override;

private:
	void _subWindowActivated(QMdiSubWindow*);

private:
	SubWindowTitleGenerator _titleGenerator = []() { return "Unnamed window"; };
	lucca::SubViewActivatedSignal _subViewActivated;
};

}

#endif // LUCCA_QT_MDI_AREA_VIEW_HPP