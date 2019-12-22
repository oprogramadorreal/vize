#ifndef LUCCA_VIEW_EVENT_HPP
#define LUCCA_VIEW_EVENT_HPP

#include "lucca/config.hpp"

#include "lucca/event/event.hpp"

#include <boost/optional.hpp>

namespace ayla {
	class Ray;
}

namespace lucca {

class View;
class Camera;

/**
 * Event generated on a lucca::View.
 *
 * @author O Programador
 */
class LUCCA_API ViewEvent : public Event {
public:
	ViewEvent(Float x, Float y, const View* sourceView, KeyboardModifierFlags keyboardModifier);
	virtual ~ViewEvent() = 0;

public:
	/**
	 * @return The position of this event in view absolute coordinates.
	 */
	glm::vec2 getPosition() const;

	/**
	 * @return The position of this event in view normalized coordinates.
	 */
	boost::optional<glm::vec2> getNormalizedPosition() const;

	/**
	 * @return The position of this event in normalized device coordinates (NDC).
	 */
	boost::optional<glm::vec2> getNDCPosition(bool invertY = true) const;

public:
	boost::optional<ayla::Ray> getCameraToViewportRay() const;
	boost::optional<glm::vec3> getCameraDirection() const;
	boost::optional<glm::mat4> getViewMatrix() const;
	boost::optional<glm::mat4> getProjectionMatrix() const;

	const View* getSourceView() const;

private:
	const Camera* _getSourceCamera() const;

private:
	const Float _x;
	const Float _y;
	const View* _sourceView;
};

}

#endif // LUCCA_VIEW_EVENT_HPP