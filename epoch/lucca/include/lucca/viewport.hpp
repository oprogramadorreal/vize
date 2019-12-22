#ifndef LUCCA_VIEWPORT_HPP
#define LUCCA_VIEWPORT_HPP

#include "lucca/config.hpp"

#include "lucca/serialization/viewport_serializer.hpp"

namespace lucca {

class RenderTarget;

/**
 * A rectangular area on a render target.
 *
 * @see lucca::RenderTarget
 *
 * @author O Programador
 */
class LUCCA_API Viewport final {
public:
	Viewport();
	Viewport(Float relativeX, Float relativeY, Float relativeWidth, Float relativeHeight);
	Viewport(Float relativeX, Float relativeY, Float relativeWidth, Float relativeHeight, RenderTarget* renderTarget);

	~Viewport();

public:
	Float getRelativeX() const;
	Float getRelativeY() const;
	Float getRelativeWidth() const;
	Float getRelativeHeight() const;

	SizeType getAbsoluteX() const;
	SizeType getAbsoluteY() const;
	SizeType getAbsoluteWidth() const;
	SizeType getAbsoluteHeight() const;

	void setRelativeX(Float value);
	void setRelativeY(Float value);
	void setRelativeWidth(Float value);
	void setRelativeHeight(Float value);

	void setRenderTarget(RenderTarget* renderTarget);

	SizeType getDefaultFramebufferObject() const;
	void postRedisplay();

private:
	RenderTarget* _renderTarget = nullptr;
	Float _relativeX = Float(0.0f);
	Float _relativeY = Float(0.0f);
	Float _relativeWidth = Float(1.0f);
	Float _relativeHeight = Float(1.0f);

	template<class Archive> friend void boost::serialization::serialize(Archive&, lucca::Viewport&, const unsigned int);
};

}

#endif // LUCCA_VIEWPORT_HPP