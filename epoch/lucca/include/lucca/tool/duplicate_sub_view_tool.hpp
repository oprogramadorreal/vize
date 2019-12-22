#ifndef LUCCA_DUPLICATE_SUB_VIEW_TOOL_HPP
#define LUCCA_DUPLICATE_SUB_VIEW_TOOL_HPP

#include "lucca/config.hpp"

#include "lucca/tool/tool_impl.hpp"

namespace lucca {

class View;

/**
 * Knows how to duplicate a lucca::View.
 * The provided view must be attached to a view composite.
 *
 * @see lucca::View
 * @see lucca::ViewComposite
 *
 * @author O Programador
 */
class LUCCA_API DuplicateSubViewTool final : public ToolImpl {
public:
	DuplicateSubViewTool(View& subView);
	virtual ~DuplicateSubViewTool();

private:
	virtual void _activateImpl() final override;

private:
	View& _subView;
};

}

#endif // LUCCA_DUPLICATE_SUB_VIEW_TOOL_HPP