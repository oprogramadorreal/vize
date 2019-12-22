#include "ayla/geometry/triangle.hpp"

namespace ayla {

glm::vec3 Triangle::getCenter() const {
	return (_a+_b+_c)*(1.0f/3.0f);
}

Float Triangle::calculateArea() const {
	return glm::length(glm::cross(_b-_a, _c-_a))*0.5f;
}

Float Triangle::calculatePerimeter() const { 
	return glm::distance(_a, _b) + glm::distance(_b, _c) + glm::distance(_c, _a); 
}

bool Triangle::intersects(const glm::vec3& p, const glm::vec3& q) const {
	// Extracted from "Real-time Collision Detection" pages 191-192

	glm::vec3 ab = _b - _a;
	glm::vec3 ac = _c - _a;
	glm::vec3 qp = p - q;

	// Compute triangle normal. Can be precalculated or cached if
	// intersecting multiple segments against the same triangle
	glm::vec3 n = glm::cross(ab, ac);

	// Compute denominator d. If d <= 0, segment is parallel to or points
	// away from triangle, so exit early
	Float d = glm::dot(qp, n);
	if (d <= 0.0f) return false;

	// Compute intersection t value of pq with plane of triangle. A ray
	// intersects iff 0 <= t. Segment intersects iff 0 <= t <= 1. Delay
	// dividing by d until intersection has been found to pierce triangle

	glm::vec3 ap = p - _a;
	Float t = glm::dot(ap, n);
	if (t < 0.0f) return false;
	if (t > d) return false; // For segment; exclude this code line for a ray test

	// Compute barycentric coordinate components and test if within bounds
	glm::vec3 e = glm::cross(qp, ap);
	Float v = glm::dot(ac, e);
	if (v < 0.0f || v > d) return false;
	Float w = -glm::dot(ab, e);
	if (w < 0.0f || v + w > d) return false;

	return true;
}

namespace {

// Real-time Collision Detection pg. 51-52
inline static Float _triArea2D(const Float& x1, const Float& y1, const Float& x2, const Float& y2, const Float& x3, const Float& y3) {
	return (x1 - x2)*(y2 - y3) - (x2 - x3)*(y1 - y2);
}

}

void Triangle::getBarycentricCoordinates(const glm::vec3& p, Float &u, Float &v, Float &w) const {
	// Extracted from "Real-time Collision Detection" pages 51-52.

	// Unnormalized triangle normal
	const glm::vec3 m = glm::cross(_b - _a, _c - _a);
	// Nominators and one-over-denominator for u and v ratios
	Float nu, nv, ood;
	// Absolute components for determining projection plane
	const Float x = std::abs(m.x), y = std::abs(m.y), z = std::abs(m.z);

	// Compute areas in plane of largest projection
	if (x >= y && x >= z){ // x is largest, project to the yz plane
		nu = _triArea2D(p.y, p.z, _b.y, _b.z, _c.y, _c.z); // Area of PBC in yz plane
		nv = _triArea2D(p.y, p.z, _c.y, _c.z, _a.y, _a.z); // Area of PCA in yz plane
		ood = 1.0f / m.x; // 1/(2*area of ABC in yz plane)

	} else if (y >= x && y >= z) {
		// y is largest, project to the xz plane
		nu = _triArea2D(p.x, p.z, _b.x, _b.z, _c.x, _c.z);
		nv = _triArea2D(p.x, p.z, _c.x, _c.z, _a.x, _a.z);
		ood = 1.0f / -m.y;

	} else { // z is largest, project to the xy plane
		nu = _triArea2D(p.x, p.y, _b.x, _b.y, _c.x, _c.y);
		nv = _triArea2D(p.x, p.y, _c.x, _c.y, _a.x, _a.y);
		ood = 1.0f / m.z;
	}

	u = nu * ood;
	v = nv * ood;
	w = 1.0f - u - v;
}

// Real-time Collision Detection pg. 51-52
bool Triangle::belongs(const glm::vec3& point) const {
	Float u, v, w;
	getBarycentricCoordinates(point, u, v, w);
	return v >= 0.0f && w >= 0.0f && (v + w) <= 1.0f;
}

bool Triangle::epsilonBelongs(const glm::vec3& point, Float epsilon) const {
	assert(epsilon >= 0.0f);
	Float u, v, w;
	getBarycentricCoordinates(point, u, v, w);
	return (v >= -epsilon) && (w >= -epsilon) && ((v + w) <= 1.0f + epsilon);
}

// Real-time Collision Detection pgs. 172-174
bool Triangle::intersects(const Triangle& other) const {
	const Plane plane = getPlane();

	const glm::vec3 a = other.getA(), b = other.getB(), c = other.getC();

	const Plane::Side aSide = plane.getSide(a);
	const Plane::Side bSide = plane.getSide(b);
	const Plane::Side cSide = plane.getSide(c);

	if (aSide == bSide) {
		if (aSide == cSide) {
			return false;		
		}
	}

	// this triangle intersects the plane defined by the other triangle

	const glm::vec3 edges[3][2] = { {b,a}, {c,b}, {a,c} };
	glm::vec3 intrsSeg[2] = { glm::vec3(0), glm::vec3(0) }; // the line segment of intersection between this triangle with other triangle's plane
	int currSeg = 0;

	for (SizeType i = 0u; i < 3u; ++i) {
		if (plane.intersection(edges[i][0], edges[i][1], intrsSeg[currSeg])) {
			if (++currSeg == 2) {
				break;
			}
		}
	}

	return intersects(intrsSeg[0], intrsSeg[1]);
}

// Real-time Collision Detection pgs 141-142
glm::vec3 Triangle::getClosestPoint(const glm::vec3& point, Float& u, Float& v, Float& w) const {
	// Check if P in vertex region outside A
	const glm::vec3 ab = _b - _a;
	const glm::vec3 ac = _c - _a;
	const glm::vec3 ap = point - _a;
	const Float d1 = glm::dot(ab, ap);
	const Float d2 = glm::dot(ac, ap);

	if (d1 <= 0.0f && d2 <= 0.0f) {
		u = 1.0f;
		v = 0.0f;
		w = 0.0f;
		return _a; // barycentric coordinates (1,0,0)
	}

	// Check if P in vertex region outside B
	const glm::vec3 bp = point - _b;
	const Float d3 = glm::dot(ab, bp);
	const Float d4 = glm::dot(ac, bp);

	if (d3 >= 0.0f && d4 <= d3) {
		u = 0.0f;
		v = 1.0f;
		w = 0.0f;
		return _b; // barycentric coordinates (0,1,0)
	}

	// Check if P in edge region of AB, if so return projection of P onto AB
	const Float vc = d1*d4 - d3*d2;

	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
		v = d1 / (d1 - d3);
		u = 1.0f - v;
		w = 0.0f;
		return _a + ab*v; // barycentric coordinates (1-v,v,0)
	}

	// Check if P in vertex region outside C
	const glm::vec3 cp = point - _c;
	const Float d5 = glm::dot(ab, cp);
	const Float d6 = glm::dot(ac, cp);

	if (d6 >= 0.0f && d5 <= d6) {
		u = 0.0f;
		v = 0.0f;
		w = 1.0f;
		return _c; // barycentric coordinates (0,0,1)
	}

	// Check if P in edge region of AC, if so return projection of P onto AC
	const Float vb = d5*d2 - d1*d6;

	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
		w = d2 / (d2 - d6);
		u = 1.0f - w;
		v = 0.0f;
		return _a + ac*w; // barycentric coordinates (1-w,0,w)
	}

	// Check if P in edge region of BC, if so return projection of P onto BC
	const Float va = d3*d6 - d5*d4;
	
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
		w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		u = 0.0f;
		v = 1.0f - w;
		return _b + (_c - _b) * w; // barycentric coordinates (0,1-w,w)
	}

	// P inside face region. Compute Q through its barycentric coordinates (u,v,w)
	const Float denom = 1.0f / (va + vb + vc);
	v = vb * denom;
	w = vc * denom;
	u = 1.0f - v - w;

	return _a + ab*v + ac*w; // = u*a + v*b + w*c, u = va* denom = 1.0f-v-w
}

void Triangle::calculateAABB(glm::vec3& minPt, glm::vec3& maxPt) const {
	minPt = glm::vec3(std::numeric_limits<Float>::max());
	maxPt = glm::vec3(-std::numeric_limits<Float>::max());

	for (SizeType i = 0u; i < 3u; ++i) {
		const glm::vec3& vi = (*this)[i];

		for (SizeType j = 0u; j < 3u; ++j) {
			if (vi[j] < minPt[j]) {
				minPt[j] = vi[j];
			}

			if (vi[j] > maxPt[j]) {
				maxPt[j] = vi[j];
			}
		}
	}
}

Sphere Triangle::calculateBoundingSphere() const {
	glm::vec3 c = getCenter();
	Float maxDist = -std::numeric_limits<Float>::max();

	for (SizeType i = 0u; i < 3u; ++i) {
		Float dist = glm::distance((*this)[i], c);

		if (dist > maxDist) {
			maxDist = dist;
		}
	}

	return Sphere(c, maxDist);
}

}
