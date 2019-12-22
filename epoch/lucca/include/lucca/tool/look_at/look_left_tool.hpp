#ifndef LUCCA_LOOK_LEFT_TOOL_HPP
#define LUCCA_LOOK_LEFT_TOOL_HPP

#include "lucca/config.hpp"

#include "lucca/tool/look_at/look_at_tool.hpp"

namespace lucca {

/**
 * @author O Programador
 */
class LUCCA_API LookLeftTool final : public LookAtTool {
public:
	LookLeftTool(View& view);
	virtual ~LookLeftTool();

private:
	virtual glm::mat4 _calculateViewMatrix(Float distance, const glm::vec3& target) const final override;
};

}

#endif // LUCCA_LOOK_LEFT_TOOL_HPP