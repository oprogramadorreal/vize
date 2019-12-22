#ifndef AYLA_RAY_HPP
#define AYLA_RAY_HPP

#include "ayla/config.hpp"

#include "ayla/serialization/ray_serializer.hpp"

namespace ayla {

/**
 * A ray defined by a starting point and a direction to infinity.
 * Direction must have size 1 ( i.e. it must be normalized ). 
 *
 * @author O Programador
 */
class AYLA_API Ray final {
public:
	Ray() = default;

	Ray(const glm::vec3 &origin, const glm::vec3 &direction);
	Ray(const Ray& other);

	inline const glm::vec3& getOrigin() const { return _origin; }
	inline const glm::vec3& getDirection() const { return _direction; }

	inline void setOrigin(const glm::vec3& o) { _origin = o; }
	inline void setDirection(const glm::vec3& d) { _direction = d; }
	
	glm::vec3 getPointAt(const Float &distance) const;

	inline glm::vec3 getFarPoint() const {
		return getPointAt(getFarDistance());
	}

	inline Float getFarDistance() const { return 1000000.0f; }

	/**
	 * @warning Not tested! TODO unit test
	 */
	Float distanceTo(const glm::vec3& pt, const Float& maxDist) const;

	friend std::ostream& operator<<(std::ostream &out, const Ray& ray);

private:
	glm::vec3 _origin = glm::vec3(0.0f);
	glm::vec3 _direction = glm::vec3(0.0f);

	template<class Archive> friend void boost::serialization::serialize(Archive&, ayla::Ray&, const unsigned int);
};

}

#endif // AYLA_RAY_HPP
