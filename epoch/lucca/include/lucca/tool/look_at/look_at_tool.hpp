#ifndef LUCCA_LOOK_AT_TOOL_HPP
#define LUCCA_LOOK_AT_TOOL_HPP

#include "lucca/config.hpp"

#include "lucca/tool/tool_impl.hpp"

namespace lucca {

class View;
class Camera;

/**
 * @author O Programador
 */
class LUCCA_API LookAtTool : public ToolImpl {
public:
	LookAtTool(View& view);
	virtual ~LookAtTool();

public:
	glm::vec3 getCameraTarget() const;

private:
	virtual void _activateImpl() final override;

private:
	virtual glm::mat4 _calculateViewMatrix(Float distance, const glm::vec3& target) const = 0;

private:
	Float _calculateBestCameraDistance(const Camera& camera) const;

private:
	View& _view;
};

}

#endif // LUCCA_LOOK_AT_TOOL_HPP