#include "ayla/geometry/line_segment.hpp"
#include "ayla/geometry/vector.hpp"

#include <glm/gtc/epsilon.hpp>

namespace ayla {

// Real-time collision detection pg 128
glm::vec3 LineSegment::getClosestPoint(const glm::vec3& point, Float& t) const {
	const glm::vec3 pq = getPQ();

	t = glm::dot(point - getP(), pq) / glm::dot(pq, pq);

	t = glm::clamp(t, 0.0f, 1.0f);

	return getP() + t * pq;
}

bool LineSegment::epsilonEqual(const LineSegment& other, Float epsilon) const {
	return ayla::epsilonEqual(_p, other._p, epsilon) && ayla::epsilonEqual(_q, other._q, epsilon);
}

namespace {

bool _intersectsAux(const LineSegment& a, const LineSegment& b, glm::vec3& interPt) {
	// http://stackoverflow.com/questions/2316490/the-algorithm-to-find-the-point-of-intersection-of-two-3d-line-segment

	const glm::vec3 da = a.getPQ();
	const glm::vec3 db = b.getPQ();
	const glm::vec3 dc = b.getP() - a.getP();

	const glm::vec3 daCrossDb = glm::cross(da, db);

	if (!isZero(glm::dot(dc, daCrossDb))) {
		return false; // lines are not coplanar
	}

	const Float s = glm::dot(glm::cross(dc, db), daCrossDb) / glm::length2(daCrossDb);

	if (s >= 0.0f && s <= 1.0f) {
		interPt = a.getP() + (da * s);
		return true;
	}

	return false;
}

}

bool LineSegment::intersects(const LineSegment& other, glm::vec3& interPt) const {
	return _intersectsAux(*this, other, interPt) && _intersectsAux(other, *this, interPt);
}

Float LineSegment::squaredDistanceTo(const glm::vec3& pt) const {
	// http://www.softsurfer.com/Archive/algorithm_0102/algorithm_0102.htm

	const glm::vec3 v = _q - _p;
	const glm::vec3 w = pt - _p;
	const Float c1 = glm::dot(w, v);
	const Float c2 = glm::dot(v, v);

	if (c1 <= 0.0f) {
		return glm::distance2(pt, _p);
	}

	if (c2 <= c1) {
		return glm::distance2(pt, _q);
	}

	const Float b = c1/c2;
	const glm::vec3 pb = _p + b * v;

	return glm::distance2(pt, pb);
}

namespace {

// clamp n to lie within the range [min, max]
inline static Float _clamp(Float n, Float min, Float max) {
	if (n < min) return min;
	if (n > max) return max;
	return n;
}

}

Float LineSegment::getClosestPoints(const LineSegment& other, glm::vec3& c1, glm::vec3& c2, Float& s, Float& t, Float epsilon) const {
	// Extracted from "Real-time Collision Detection" page 149.

	const glm::vec3 d1 = this->_q - this->_p; // Direction vector of this segment
	const glm::vec3 d2 = other._q - other._p; // Direction vector of other segment
	const glm::vec3 r = this->_p - other._p;
	const Float a = glm::dot(d1, d1); // Squared length of this segment, always nonnegative
	const Float e = glm::dot(d2, d2); // Squared length of other segment, always nonnegative
	const Float f = glm::dot(d2, r);

	// Check if either or both segments degenerate into points
	if (a <= epsilon && e <= epsilon) {
		// Both segments degenerate into points
		s = t = 0.0f;
		c1 = this->_p;
		c2 = other._p;
		return glm::dot(c1 - c2, c1 - c2);
	}

	if (a <= epsilon) {
		// First segment degenerates into a point
		s = 0.0f;
		t = f / e; // s = 0 => t = (b*s + f) / e = f / e
		t = _clamp(t, 0.0f, 1.0f);

	} else {
		const Float c = glm::dot(d1, r);

		if (e <= epsilon) {
			// Second segment degenerates into a point
			t = 0.0f;
			s = _clamp(-c / a, 0.0f, 1.0f); // t = 0 => s = (b*t - c) / a = -c / a

		} else {
			// The general nondegenerate case starts here
			const Float b = glm::dot(d1, d2);
			const Float denom = a*e - b*b; // Always nonnegative

			// If segments not parallel, compute closest point on L1 to L2 and
			// clamp to segment S1. Else pick arbitrary s (here 0)
			if (denom != 0.0f) {
				s = _clamp((b*f - c*e) / denom, 0.0f, 1.0f);
			} else {
				s = 0.0f;
			}

			// Compute point on L2 closest to S1(s) using
			// t = Dot((P1 + D1*s) - P2,D2) / Dot(D2,D2) = (b*s + f) / e
			t = (b*s + f) / e;

			// If t in [0,1] done. Else clamp t, recompute s for the new value
			// of t using s = Dot((P2 + D2*t) - P1,D1) / Dot(D1,D1)= (t*b - c) / a
			// and clamp s to [0, 1]
			if (t < 0.0f) {
				t = 0.0f;
				s = _clamp(-c / a, 0.0f, 1.0f);
			} else if (t > 1.0f) {
				t = 1.0f;
				s = _clamp((b - c) / a, 0.0f, 1.0f);
			}
		}
	}

	c1 = this->_p + d1 * s;
	c2 = other._p + d2 * t;

	return glm::dot(c1 - c2, c1 - c2);
}

std::ostream& operator<<(std::ostream &out, const LineSegment& segment) {
	out << "P1 = " << segment.getP() << " / P2 = " << segment.getQ() << std::endl;
	return out;
}

}