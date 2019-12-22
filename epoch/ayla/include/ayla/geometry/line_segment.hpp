#ifndef AYLA_LINE_SEGMENT_HPP
#define AYLA_LINE_SEGMENT_HPP

#include "ayla/config.hpp"

#include "ayla/serialization/line_segment_serializer.hpp"

#include <glm/gtx/norm.hpp>

namespace ayla {

/**
 * A line segment between two points P and Q.
 *
 * @author O Programador
 */
class AYLA_API LineSegment final {
public:
	LineSegment() = default;

	inline LineSegment(const glm::vec3& p, const glm::vec3& q)
		: _p(p), _q(q)
	{ }

	inline LineSegment(const LineSegment& other)
		: _p(other.getP()), _q(other.getQ())
	{ }

	inline ~LineSegment() { }

	inline void setP(const glm::vec3& p) { _p = p; }
	inline void setQ(const glm::vec3& q) { _q = q; }

	inline const glm::vec3& getP() const { return _p; }
	inline const glm::vec3& getQ() const { return _q; }

	inline Float length() const { return glm::distance(getP(), getQ()); }
	inline Float length2() const { return glm::distance2(getP(), getQ()); }

	inline glm::vec3 getPQ() const { return getQ() - getP(); }
	inline glm::vec3 getQP() const { return getP() - getQ(); }

	/**
	 * @param segmentFraction A scalar in the range [0,1].
	 */
	inline glm::vec3 getPointAt(Float segmentFraction) const {
		return getP() + segmentFraction * getPQ();
	}

	inline glm::vec3 getClosestPoint(const glm::vec3& point) const {
		Float t; // trash
		return getClosestPoint(point, t);
	}
	glm::vec3 getClosestPoint(const glm::vec3& point, Float& t) const;

	bool epsilonEqual(const LineSegment& other, Float epsilon = std::numeric_limits<Float>::epsilon()) const;

	inline bool intersects(const LineSegment& other) const {
		glm::vec3 trash;
		return intersects(other, trash);
	}

	bool intersects(const LineSegment& other, glm::vec3& interPt) const;

	inline glm::vec3 getCenter() const { return (getP() + getQ()) / 2.0f; }

	/**
	 * @return The distance between this line segment and @param pt.
	 */
	inline Float distanceTo(const glm::vec3& pt) const {
		return glm::sqrt(squaredDistanceTo(pt));
	}
	Float squaredDistanceTo(const glm::vec3& pt) const;

	/**
	 * Computes closest points c1 and c2 between this and @param other segment.
	 *
	 * Also calculates s and t are also calculated so that:
	 * c1 = this->getP1() + s * (this->getP2() - this->getP1())
	 * c2 = other.getP1() + t * (other.getP2() - other.getP1())
	 *
	 * @return The squared distance between closest points c1 and c2.
	 */
	Float getClosestPoints(const LineSegment& other, glm::vec3& c1, glm::vec3& c2, Float& s, Float& t, Float epsilon = 0.00000001f) const;

	/**
	 * @return The squared distance between the closest points between this segment and @param s2.
	 */
	inline Float squaredDistance(const LineSegment& s2, Float epsilon = 0.00000001f) const {
		glm::vec3 c1, c2; // trash
		Float s, t;
		return getClosestPoints(s2, c1, c2, s, t, epsilon);
	}

	friend std::ostream& operator<<(std::ostream &out, const LineSegment& segment);

private:
	glm::vec3 _p = glm::vec3(0.0f);
	glm::vec3 _q = glm::vec3(0.0f);

	template<class Archive> friend void boost::serialization::serialize(Archive&, ayla::LineSegment&, const unsigned int);
};

}

#endif // AYLA_LINE_SEGMENT_HPP
