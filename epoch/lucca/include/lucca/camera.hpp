#ifndef LUCCA_CAMERA_HPP
#define LUCCA_CAMERA_HPP

#include "lucca/config.hpp"

#include "lucca/viewport.hpp"
#include "lucca/serialization/camera_serializer.hpp"

#include <boost/signals2.hpp>

namespace ayla {
	class Ray;
}

namespace lucca {

/**
 * A viewpoint from which a scene will be rendered.
 *
 * @see lucca::RenderTarget
 *
 * @author O Programador
 */
class LUCCA_API Camera final {
public:
	Camera();
	Camera(const Viewport& viewport);
	Camera(const Camera& other);

	~Camera();

public:
	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjectionMatrix() const;

	void setViewMatrix(const glm::mat4& viewMatrix);
	void setProjectionMatrix(Float fovyRadians, Float nearDistance, Float farDistance);

	glm::vec3 getPosition() const;
	glm::vec3 getDirection() const;

	Float getFovy() const;
	Float getNearDistance() const;
	Float getFarDistance() const;

	void setRenderTarget(RenderTarget* renderTarget);

	SizeType getDefaultFramebufferObject() const;
	void postRedisplay();

public:
	SizeType getViewportAbsoluteX() const;
	SizeType getViewportAbsoluteY() const;
	SizeType getViewportAbsoluteWidth() const;
	SizeType getViewportAbsoluteHeight() const;

public:
	ayla::Ray getCameraToViewportRay(const glm::vec2& pointOnViewport) const;
	glm::vec2 getNormalizedPointOnViewport(const glm::vec2& pointOnViewport) const;

public:
	/**
	 * Emitted when this camera get any change.
	 */
	using ChangedSignal = boost::signals2::signal<void(const Camera&)>;

	boost::signals2::connection connectToChangedSignal(ChangedSignal::slot_function_type slot);

private:
	Viewport _viewport;
	glm::mat4 _viewMatrix = glm::mat4(1.0f);
	Float _fovyRadians = Float(glm::radians(45.0f));
	Float _nearDistance = Float(0.1f);
	Float _farDistance = Float(1000.0f);
	ChangedSignal _signalChanged;

	template<class Archive> friend void boost::serialization::serialize(Archive&, lucca::Camera&, const unsigned int);
};

}

#endif // LUCCA_CAMERA_HPP