#ifndef AYLA_CYLINDER_HPP
#define AYLA_CYLINDER_HPP

#include "ayla/config.hpp"

#include "ayla/geometry/line_segment.hpp"
#include "ayla/serialization/cylinder_serializer.hpp"

namespace ayla {

/**
 * @author O Programador
 */
class AYLA_API Cylinder final {
public:
	inline Cylinder(const LineSegment& axis, const Float& radius)
		: _axis(axis), _radius(radius)
	{ }

	inline Cylinder()
		: _radius(0.0f)
	{ }
	
	inline const LineSegment& getAxis() const {
		return _axis;
	}
	
	inline const Float& getRadius() const {
		return _radius;
	}

	bool contains(const glm::vec3& point) const;
	
private:
	LineSegment _axis;
	Float _radius = 0.0f;

	template<class Archive> friend void boost::serialization::serialize(Archive&, ayla::Cylinder&, const unsigned int);
};

}

#endif // AYLA_CYLINDER_HPP