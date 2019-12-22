#include "ayla/geometry/intersection.hpp"

namespace ayla {
namespace intersection {

// Real-Time Collision Detection pgs 180-181
bool rayAab(const Ray& r, const AxisAlignedBox& a, Float &distance) {
	const Float epsilon = 0.0001f;

	Float tmin(0.0f); // set to -FLT_MAX to get first hit on line
	Float tmax(std::numeric_limits<Float>::infinity()); // set to max distance ray can travel (for segment)

	const glm::vec3 amin(a.getMinPoint());
	const glm::vec3 amax(a.getMaxPoint());

	const glm::vec3 rOri(r.getOrigin());
	const glm::vec3 rDir(r.getDirection());

	// For all three slabs
	for (int i = 0; i < 3; i++) {
		if (std::abs(rDir[i]) < epsilon) {
			// Ray is parallel to slab. No hit if r.origin not within slab
			if ((rOri[i] < amin[i]) || (rOri[i] > amax[i])) {
				return false;
			}
		} else {
			// Compute intersection t value of ray with near and far plane of slab
			Float ood(Float(1.0f) / rDir[i]);
			Float t1((amin[i] - rOri[i]) * ood);
			Float t2((amax[i] - rOri[i]) * ood);

			// Make t1 be intersection with near plane, t2 with far plane
			if (t1 > t2) std::swap(t1, t2);

			// Compute the intersection of slab intersection intervals
			tmin = std::max(tmin, t1); // Rather than: if (t1 > tmin) tmin = t1;
			tmax = std::min(tmax, t2); // Rather than: if (t2 < tmax) tmax = t2;

			// Exit with no collision as soon as slab intersection becomes empty
			if (tmin > tmax) {
				return false;
			}
		}
	}

	// Ray intersects all 3 slabs. Return point (q) and intersection t value (tmin)
	distance = tmin;

	return true;
}

bool rayBox(const Ray& r, const Box& box, Float &distance) {
	RigidTransform t(box.getTransform());
	Ray newRay(t.transformInverse(r.getOrigin()), t.transformInverseDirection(r.getDirection()));
	const glm::vec3 he(box.getHalfExtensions());
	AxisAlignedBox a(glm::vec3(0), he.x, he.y, he.z);
	return rayAab(newRay, a, distance);
}

bool raySphere(const Ray& r, const Sphere& s, Float &distance) {
	const glm::vec3 m = r.getOrigin() - s.getCenter();
	const Float b(glm::dot(m, r.getDirection()));
	const Float c(glm::dot(m, m) - s.getRadius() * s.getRadius());

	// Exit if r’s r.origin outside s (c > 0) and r pointing away from s (b > 0)
	if ((c > Float(0.0f)) && (b > Float(0.0f))) {
		return false;
	}

	const Float discr = b*b - c;

	// A negative discriminant corresponds to ray missing s
	if (discr < Float(0.0f)) {
		return false;
	}

	// Ray now found to intersect s, compute smallest t value of intersection
	distance = -b - std::sqrt(discr);

	// If t is negative, ray started inside s so clamp t to zero
	if (distance < 0.0f) {
		distance = Float(0.0f);
	}

	return true;
}

bool rayPlane(const Ray& r, const Plane& p, Float &distance) { // TODO improve this
	glm::vec3 intrsPt(0);
	const bool ok = planeLineSegment(p, LineSegment(r.getOrigin(), r.getFarPoint()), intrsPt);
	distance = glm::distance(intrsPt, r.getOrigin());
	return ok;
}

bool sphereAab(const Sphere& s, const AxisAlignedBox& a) {
	// Real-Time Collision Detection pgs 165-166

	// Compute squared distance between sphere center and AABB
	const Float sqDist = a.getSqrDistance(s.getCenter());
	// Sphere and AABB intersect if the (squared) distance
	// between them is less than the (squared) sphere radius
	return sqDist <= (s.getRadius() * s.getRadius());
}

// Real-time Collision Detection pgs 191
// Given segment pq and triangle abc, returns whether segment intersects
// triangle and if so, also returns the barycentric coordinates (u,v,w)
// of the intersection point
bool triangleLineSegment(const Triangle& tri, const LineSegment& lArg, glm::vec3& intrsPt) {
	LineSegment l = lArg;
	const glm::vec3 ab = tri.getB() - tri.getA();
	const glm::vec3 ac = tri.getC() - tri.getA();
	glm::vec3 qp = l.getQP();

	// Compute triangle normal. Can be precalculated or cached if
	// intersecting multiple segments against the same triangle
	const glm::vec3 n = glm::cross(ab, ac);

	// Compute denominator d. If d <= 0, segment is parallel to or points
	// away from triangle, so exit early
	Float d = glm::dot(qp, n);
	if (d == 0.0f) {
		if( lArg.intersects( LineSegment( tri.getA(), tri.getB() ), intrsPt ) )
			return true;
		if( lArg.intersects( LineSegment( tri.getA(), tri.getC() ), intrsPt ) )
			return true;
		if( lArg.intersects( LineSegment( tri.getB(), tri.getC() ), intrsPt ) )
			return true;
		if( tri.belongs( lArg.getP() ) && tri.belongs( lArg.getQ() ) ){
			intrsPt = lArg.getP();
			return true;
		}
		return false;
	}
	if (d < 0.0f) {
		d = -d;
		qp = -qp;
		l = LineSegment(l.getQ(), l.getP());
	}

	// Compute intersection t value of pq with plane of triangle. A ray
	// intersects iff 0 <= t. Segment intersects iff 0 <= t <= 1. Delay
	// dividing by d until intersection has been found to pierce triangle Note that if d < 0 the segment points away from the triangle, and if d = 0 the
	const glm::vec3 ap = l.getP() - tri.getA();
	Float t = glm::dot(ap, n);
	if (t < 0.0f) return false;
	if (t > d) return false; // For segment; exclude this code line for a ray test

	// Compute barycentric coordinate components and test if within bounds
	const glm::vec3 e = glm::cross(qp, ap);
	Float v = glm::dot(ac, e);
	if ((v < 0.0f) || (v > d)) return false;
	Float w = -glm::dot(ab, e);
	if ((w < 0.0f) || (v + w) > d) return false;

	// Segment/ray intersects triangle. Perform delayed division and
	// compute the last barycentric coordinate component
	const Float ood = 1.0f / d;
	t *= ood;
	v *= ood;
	w *= ood;
	const Float u = 1.0f - v - w;

	intrsPt = u*tri.getA() + v*tri.getB() + w*tri.getC();

	return true;
}

// Real-time Collision Detection pgs 191-192
// Given segment pq and triangle abc, returns whether segment intersects triangle
bool triangleLineSegment(const Triangle& tri, const LineSegment& lArg) {
	LineSegment l = lArg;
	glm::vec3 ab = tri.getB() - tri.getA();
	glm::vec3 ac = tri.getC() - tri.getA();
	glm::vec3 qp = l.getQP();

	// Compute triangle normal. Can be precalculated or cached if
	// intersecting multiple segments against the same triangle
	glm::vec3 n = glm::cross(ab, ac);

	// Compute denominator d. If d <= 0, segment is parallel to or points
	// away from triangle, so exit early
	Float d = glm::dot(qp, n);
	if (d == 0.0f) {
		if( lArg.intersects( LineSegment( tri.getA(), tri.getB() ) ) )
			return true;
		if( lArg.intersects( LineSegment( tri.getA(), tri.getC() ) ) )
			return true;
		if( lArg.intersects( LineSegment( tri.getB(), tri.getC() ) ) )
			return true;	
		if( tri.belongs( lArg.getP() ) && tri.belongs( lArg.getQ() ) )
			return true;
		return false;
	}
	if (d < 0.0f) {
		d = -d;
		qp = -qp;
		l = LineSegment(l.getQ(), l.getP());
	}

	// Compute intersection t value of pq with plane of triangle. A ray
	// intersects iff 0 <= t. Segment intersects iff 0 <= t <= 1. Delay
	// dividing by d until intersection has been found to pierce triangle

	glm::vec3 ap = l.getP() - tri.getA();
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

// Real-time Collision Detection pg. 176
// Compute the t value for the directed line ab intersecting the plane
bool planeLineSegment(const Plane& plane, const LineSegment& segment, glm::vec3& intersectionPoint) {
	const glm::vec3 pq = segment.getQ() - segment.getP();
	const Float dot = glm::dot(plane.getNormal(), pq);

	if (ayla::isZero(dot)) {
		if (ayla::isZero(plane.getDistance(segment.getP()))) {
			// special case when segment belongs to the plane... return segment P point as intersection
			intersectionPoint = segment.getP();
			return true;
		}

		return false; // no intersection
	}

	const Float t = (plane.getD() - glm::dot(plane.getNormal(), segment.getP())) / dot;

	// regular case: if t in [0..1] compute and return intersection point
	if (t >= 0.0f && t <= 1.0f) {
		intersectionPoint = segment.getP() + t * pq;
		return true;
	}

	return false; // no intersection
}

// Real-time Collision Detection pg. 183-184
bool aabLineSegment(const AxisAlignedBox& box, const LineSegment& l) {
	const glm::vec3 c = (box.getMinPoint() + box.getMaxPoint()) * 0.5f; // Box center-point
	const glm::vec3 e = box.getMaxPoint() - c; // Box halflength extents
	glm::vec3 m = (l.getP() + l.getQ()) * 0.5f; // Segment midpoint
	const glm::vec3 d = l.getQ() - m; // Segment halflength vector
	m = m - c; // Translate box and segment to origin

	// Try world coordinate axes as separating axes
	Float adx = std::abs(d.x);
	if (std::abs(m.x) > e.x + adx) return false;
	Float ady = std::abs(d.y);
	if (std::abs(m.y) > e.y + ady) return false;
	Float adz = std::abs(d.z);
	if (std::abs(m.z) > e.z + adz) return false;

	const Float epsilon = std::numeric_limits<Float>::epsilon();

	// Add in an epsilon term to counteract arithmetic errors when segment is
	// (near) parallel to a coordinate axis (see text for detail)
	adx += epsilon; ady += epsilon; adz += epsilon;
	// Try cross products of segment direction vector with coordinate axes
	if (Float(m.y * d.z - m.z * std::abs(d.y)) > e.y * adz + e.z * ady) return false;
	if (Float(m.z * d.x - m.x * std::abs(d.z)) > e.x * adz + e.z * adx) return false;
	if (Float(m.x * d.y - m.y * std::abs(d.x)) > e.x * ady + e.y * adx) return false;

	// No separating axis found; segment must be overlapping AABB
	return true;
}

bool aabPlane(const AxisAlignedBox& aab, const Plane& plane) {
	const glm::vec3 c = aab.getCenter();
	const glm::vec3 e = aab.getMaxPoint() - c;
	const glm::vec3 pn = plane.getNormal();

	const Float r = e[0] * std::abs(pn[0]) + e[1] * std::abs(pn[1]) + e[2] * std::abs(pn[2]);
	const Float s = glm::dot(pn, c) - plane.getD();
	
	const Float magnitudeS = std::abs(s);
	
	if ( ayla::isZero(magnitudeS - r, AYLA_PLANE_DEFAULT_EPSILON) ) 
		return true;
	return magnitudeS < r;
}

bool aabTriangle(const AxisAlignedBox& aab, const Triangle& t) {
	// real-time collision detection pg 171

	Float p0, p1, p2, r;

	// Compute box center and extents
	const glm::vec3 c = aab.getCenter();
	const Float e0 = aab.getHalfWidth();
	const Float e1 = aab.getHalfHeight();
	const Float e2 = aab.getHalfDepth();

	// Translate triangle as conceptually moving AABB to origin
	const glm::vec3 v0 = t.getA() - c;
	const glm::vec3 v1 = t.getB() - c;
	const glm::vec3 v2 = t.getC() - c;

	// Compute edge vectors for triangle
	const glm::vec3 f0 = v1 - v0, f1 = v2 - v1, f2 = v0 - v2;
	// Test axes a00..a22 (category 3)

	// Test axis a00
	p0 = v0.z*v1.y - v0.y*v1.z;
	p2 = v2.z*(v1.y - v0.y) - v2.z*(v1.z - v0.z);
	r = e1 * std::abs(f0.z) + e2 * std::abs(f0.y);
	if (std::max(-std::max(p0, p2), std::min(p0, p2)) > r) return false; // Axis is a separating axis

	// Test axis a01
	p0 = v0.z*(v2.y - v1.y) - v0.y*(v2.z - v1.z);
	p1 = v1.z*v2.y - v1.y*v2.z;
	r = e1 * fabs(f1.z) + e2 * fabs(f1.y);
	if (std::max(-std::max(p0, p1), std::min(p0, p1)) > r) return false; // Axis is a separating axis

	// Test axis a02
	p0 = v0.y*v2.z - v0.z*v2.y;
	p1 = v1.z*(v0.y - v2.y) - v1.y*(v0.z - v2.z);
	r = e1 * fabs(f2.z) + e2 * fabs(f2.y);
	if (std::max(-std::max(p0, p1), std::min(p0, p1)) > r) return false; // Axis is a separating axis

	// Test axis a10
	p0 = v0.x*v1.z - v0.z*v1.x;
	p2 = v2.x*(v1.z - v0.z) - v2.z*(v1.x - v0.x);
	r = e0 * fabs(f0.z) + e2 * fabs(f0.x);
	if (std::max(-std::max(p0, p2), std::min(p0, p2)) > r) return false; // Axis is a separating axis

	// Test axis a11
	p0 = v0.x*(v2.z - v1.z) - v0.z*(v2.x - v1.x);
	p1 = v1.x*v2.z - v1.z*v2.x;
	r = e0 * fabs(f1.z) + e2 * fabs(f1.x);
	if (std::max(-std::max(p0, p1), std::min(p0, p1)) > r) return false; // Axis is a separating axis

	// Test axis a12
	p0 = v0.z*v2.x - v0.x*v2.z;
	p1 = v1.x*(v0.z - v2.z) - v1.z*(v0.x - v2.x);
	r = e0 * fabs(f1.z) + e2 * fabs(f1.x);
	if (std::max(-std::max(p0, p1), std::min(p0, p1)) > r) return false; // Axis is a separating axis

	// Test axis a20
	p0 = v0.y*v1.x - v0.x*v1.y;
	p2 = v2.y*(v1.x - v0.x) - v2.x*(v1.y - v0.y);
	r = e0 * fabs(f0.y) + e1 * fabs(f0.x);
	if (std::max(-std::max(p0, p2), std::min(p0, p2)) > r) return false; // Axis is a separating axis

	// Test axis a21
	p0 = v0.y*(v2.x - v1.x) - v0.x*(v2.y - v1.y);
	p1 = v1.y*v2.x - v1.x*v2.y;
	r = e0 * fabs(f1.y) + e1 * fabs(f1.x);
	if (std::max(-std::max(p0, p1), std::min(p0, p1)) > r) return false; // Axis is a separating axis

	// Test axis a22
	p0 = v0.x*v2.y - v0.y*v2.x;
	p1 = v1.y*(v0.x - v2.x) - v1.x*(v0.y - v2.y);
	r = e0 * fabs(f1.y) + e1 * fabs(f1.x);
	if (std::max(-std::max(p0, p1), std::min(p0, p1)) > r) return false; // Axis is a separating axis

	// Test the three axes corresponding to the face normals of AABB b (category 1).
	// Exit if...
	// ... [-e0, e0] and [min(v0.x,v1.x,v2.x), max(v0.x,v1.x,v2.x)] do not overlap
	if (std::max(std::max(v0.x, v1.x), v2.x) < -e0 || std::min(std::min(v0.x, v1.x), v2.x) > e0) return false;
	// ... [-e1, e1] and [min(v0.y,v1.y,v2.y), max(v0.y,v1.y,v2.y)] do not overlap
	if (std::max(std::max(v0.y, v1.y), v2.y) < -e1 || std::min(std::min(v0.y, v1.y), v2.y) > e1) return false;
	// ... [-e2, e2] and [min(v0.z,v1.z,v2.z), max(v0.z,v1.z,v2.z)] do not overlap
	if (std::max(std::max(v0.z, v1.z), v2.z) < -e2 || std::min(std::min(v0.z, v1.z), v2.z) > e2) return false;

	// Test separating axis corresponding to triangle face normal (category 2)
	const glm::vec3 pNormal = glm::normalize(glm::cross(f0, f1));
	const Float pDist = glm::dot(pNormal, v0);

	const AxisAlignedBox aab2(aab.getMinPoint() - c, aab.getMaxPoint() - c);
	return aabPlane(aab2, Plane(pNormal, pDist));
}

}
}