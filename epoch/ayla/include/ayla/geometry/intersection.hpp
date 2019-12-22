#ifndef AYLA_INTERSECTION_HPP
#define AYLA_INTERSECTION_HPP

#include "ayla/config.hpp"

#include "ayla/geometry/ray.hpp"
#include "ayla/geometry/axis_aligned_box.hpp"
#include "ayla/geometry/sphere.hpp"
#include "ayla/geometry/triangle.hpp"
#include "ayla/geometry/box.hpp"

namespace ayla {

/**
 * Functions that perform intersection tests between different geometric primitives.
 *
 * @remarks To test intersection between two equal primitives (e.g., sphere against sphere), look for a
 * method on the primitive class. Here you will find tests only between two different geometric primitives.
 *
 * Most of these tests are based on the book of Christer Ericson, "Real-Time Collision Detection".
 * @see http://realtimecollisiondetection.net
 */
namespace intersection {

AYLA_API bool rayAab(const Ray& r, const AxisAlignedBox& a, Float &distance);
AYLA_API bool rayBox(const Ray& r, const Box& box, Float &distance);
AYLA_API bool raySphere(const Ray& r, const Sphere& s, Float &distance);
AYLA_API bool rayPlane(const Ray& r, const Plane& p, Float &distance);

AYLA_API bool sphereAab(const Sphere& s, const AxisAlignedBox& a);

/**
 * Performs an intersection test between triangle t and line segment through points pq.
 * Also calculates de intersection point.
 *
 * @remarks Triangle must be counterclockwise specified.
 */
AYLA_API bool triangleLineSegment(const Triangle& t, const LineSegment& l, glm::vec3& intrsPt);

/**
 * Performs an intersection test between triangle t and line segment through points pq.
 */
AYLA_API bool triangleLineSegment(const Triangle& t, const LineSegment& l);

/**
 *  Performs an intersection test between the 'plane' and line segment through points ab.
 *
 * @remarks If @param segment belongs to @param plane, it will return TRUE and @param intersectionPoint will be the first point of the segment (point P).
 */
AYLA_API bool planeLineSegment(const Plane& plane, const LineSegment& segment, glm::vec3& intersectionPoint);

AYLA_API bool aabLineSegment(const AxisAlignedBox& box, const LineSegment& l);

/**
 * @todo Test it.
 */
AYLA_API bool aabPlane(const AxisAlignedBox& aab, const Plane& plane);

/**
 * @todo Test it.
 */
AYLA_API bool aabTriangle(const AxisAlignedBox& aab, const Triangle& t);

}
}

#endif // AYLA_INTERSECTION_HPP