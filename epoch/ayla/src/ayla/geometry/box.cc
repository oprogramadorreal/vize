#include "ayla/geometry/box.hpp"

namespace ayla {

Box::Box() = default;

Box::Box(const Box& other)
	: _aab(other._aab), _base(other._base)
{ }

Box::Box(const AxisAlignedBox& aab)
	: _aab(aab)
{ }

Box::Box(const glm::vec3& center, const Float& halfWidth, const Float& halfHeight, const Float& halfDepth, const glm::mat3& base)
	: _aab(center, halfWidth, halfHeight, halfDepth), _base(base)
{ }

RigidTransform Box::getTransform() const {
	return RigidTransform(getCenter(), _base);
}

void Box::setTransform(const RigidTransform& t) {
	_aab.setCenter(t.getTranslation());
	_base = t.getRotationMatrix();
}

std::array<glm::vec3, 8> Box::getVertices() const {
	const glm::vec3 center = getCenter();

	std::array<glm::vec3, 8> vertices = _aab.getVertices();

	for (auto it = vertices.begin(); it != vertices.end(); ++it) {
		*it = _base * (*it - center) + center;
	}

	return vertices;
}

std::array<LineSegment, 12> Box::getEdges() const {
	std::array<glm::vec3, 8> vertices = getVertices();

	std::array<LineSegment, 12> edges;
	edges[0] = LineSegment(vertices[0], vertices[1]);
	edges[1] = LineSegment(vertices[2], vertices[3]);
	edges[2] = LineSegment(vertices[6], vertices[7]);
	edges[3] = LineSegment(vertices[4], vertices[5]);
	edges[4] = LineSegment(vertices[1], vertices[5]);
	edges[5] = LineSegment(vertices[3], vertices[7]);
	edges[6] = LineSegment(vertices[2], vertices[6]);
	edges[7] = LineSegment(vertices[0], vertices[4]);
	edges[8] = LineSegment(vertices[0], vertices[2]);
	edges[9] = LineSegment(vertices[1], vertices[3]);
	edges[10] = LineSegment(vertices[5], vertices[7]);
	edges[11] = LineSegment(vertices[4], vertices[6]);

	return edges;
}

AxisAlignedBox Box::calculateAABB() const {
	const glm::vec3 u0e(glm::column(_base, 0) * getHalfWidth());
	const glm::vec3 u1e(glm::column(_base, 1) * getHalfHeight());
	const glm::vec3 u2e(glm::column(_base, 2) * getHalfDepth());

	const glm::vec3 c = getCenter();

	const glm::vec3 verts[8] = {
		c + u0e + u1e + u2e,
		c + u0e + u1e - u2e,
		c + u0e - u1e + u2e,
		c + u0e - u1e - u2e,
		c - u0e + u1e + u2e,
		c - u0e + u1e - u2e,
		c - u0e - u1e + u2e,
		c - u0e - u1e - u2e
	};

	glm::vec3 minPt(verts[0]);
	glm::vec3 maxPt(minPt);

	for (SizeType i = 1u; i < 8u; ++i) {
		for (SizeType j = 0u; j < 3u; ++j) {
			const Float tmp = verts[i][j];

			if (tmp < minPt[j]) {
				minPt[j] = tmp;
			} else if (tmp > maxPt[j]) {
				maxPt[j] = tmp;
			}
		}
	}

	return AxisAlignedBox(minPt, maxPt);
}

void Box::getDirExtremePts(const glm::vec3& normDir, glm::vec3& posDirPt, glm::vec3& negDirPt) const {
	int s[3] = {
		(normDir.x < 0.0f) ? -1 : 1,
		(normDir.y < 0.0f) ? -1 : 1,
		(normDir.z < 0.0f) ? -1 : 1
	};

	posDirPt = getCenter() + _base * glm::vec3(
		Float(s[0] * getHalfWidth()),
		Float(s[1] * getHalfHeight()),
		Float(s[2] * getHalfDepth())
	);

	negDirPt = getCenter() + _base * glm::vec3(
		Float(-s[0] * getHalfWidth()),
		Float(-s[1] * getHalfHeight()),
		Float(-s[2] * getHalfDepth())
	);
}

// Float-Time Collision Detection -- pg. 103
bool Box::intersects(const Box& other) const {
	Float ra, rb;
	glm::mat3 R, AbsR;

	const glm::vec3 ae = getHalfExtensions();
	const glm::vec3 be = other.getHalfExtensions();

	// Compute rotation matrix expressing b in a’s coordinate frame
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			R[i][j] = glm::dot(getAxis(i), other.getAxis(j));

	// Compute translation vector t
	glm::vec3 t = other.getCenter() - getCenter();
	// Bring translation into a’s coordinate frame
	t = glm::vec3( glm::dot(t, getAxis(0)), glm::dot(t, getAxis(1)), glm::dot(t, getAxis(2)) );

	// Compute common subexpressions. Add in an epsilon term to
	// counteract arithmetic errors when two edges are parallel and
	// their cross product is (near) null (see text for details)
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			AbsR[i][j] = std::abs(R[i][j]) + std::numeric_limits<Float>::epsilon();

	// Test axes L = A0, L = A1, L = A2
	for (int i = 0; i < 3; i++) {
		ra = ae[i];
		rb = be[0] * AbsR[i][0] + be[1] * AbsR[i][1] + be[2] * AbsR[i][2];
		if (std::abs(t[i]) > ra + rb) return false;
	}

	// Test axes L = B0, L = B1, L = B2
	for (int i = 0; i < 3; i++) {
		ra = ae[0] * AbsR[0][i] + ae[1] * AbsR[1][i] + ae[2] * AbsR[2][i];
		rb = be[i];
		if (std::abs(t[0] * R[0][i] + t[1] * R[1][i] + t[2] * R[2][i]) > ra + rb) return false;
	}

	// Test axis L = A0 x B0
	ra = ae[1] * AbsR[2][0] + ae[2] * AbsR[1][0];
	rb = be[1] * AbsR[0][2] + be[2] * AbsR[0][1];
	if (std::abs(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb) return false;

	// Test axis L = A0 x B1
	ra = ae[1] * AbsR[2][1] + ae[2] * AbsR[1][1];
	rb = be[0] * AbsR[0][2] + be[2] * AbsR[0][0];
	if (std::abs(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb) return false;

	// Test axis L = A0 x B2
	ra = ae[1] * AbsR[2][2] + ae[2] * AbsR[1][2];
	rb = be[0] * AbsR[0][1] + be[1] * AbsR[0][0];
	if (std::abs(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb) return false;

	// Test axis L = A1 x B0
	ra = ae[0] * AbsR[2][0] + ae[2] * AbsR[0][0];
	rb = be[1] * AbsR[1][2] + be[2] * AbsR[1][1];
	if (std::abs(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb) return false;

	// Test axis L = A1 x B1
	ra = ae[0] * AbsR[2][1] + ae[2] * AbsR[0][1];
	rb = be[0] * AbsR[1][2] + be[2] * AbsR[1][0];
	if (std::abs(t[0] * R[2][1] - t[2] * R[0][1]) > ra + rb) return false;

	// Test axis L = A1 x B2
	ra = ae[0] * AbsR[2][2] + ae[2] * AbsR[0][2];
	rb = be[0] * AbsR[1][1] + be[1] * AbsR[1][0];
	if (std::abs(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb) return false;

	// Test axis L = A2 x B0
	ra = ae[0] * AbsR[1][0] + ae[1] * AbsR[0][0];
	rb = be[1] * AbsR[2][2] + be[2] * AbsR[2][1];
	if (std::abs(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb) return false;

	// Test axis L = A2 x B1
	ra = ae[0] * AbsR[1][1] + ae[1] * AbsR[0][1];
	rb = be[0] * AbsR[2][2] + be[2] * AbsR[2][0];
	if (std::abs(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb) return false;

	// Test axis L = A2 x B2
	ra = ae[0] * AbsR[1][2] + ae[1] * AbsR[0][2];
	rb = be[0] * AbsR[2][1] + be[1] * AbsR[2][0];
	if (std::abs(t[1] * R[0][2] - t[0] * R[1][2]) > ra + rb) return false;

	return true;
}

bool Box::collideWithPoint(const glm::vec3& point, glm::vec3& normal, Float& penetration) const {
	const glm::vec3 c = getCenter();
	const glm::vec3 e = getHalfExtensions();

	const glm::vec3 d = point - c;
	glm::vec3 q = point;

	Float penOnAxis[3];
	Float smallPen = std::numeric_limits<Float>::infinity();
	int bestAxis = 0;

	// for each OBB axis
	for (int i = 0; i < 3; ++i) {
		const glm::vec3 u = getAxis(i);

		// ...project d onto that axis to get the distance
		// along the axis of d from the box center
		Float dist = glm::dot(d, u);

		// If distance farther than the box extents, no collision!
		if (dist > e[i]) return false;
		if (dist < -e[i]) return false;

		penOnAxis[i] = (dist < 0.0f) ? -(dist + Float(e[i])) : (Float(e[i]) - dist);

		// Step that distance along the axis to get world coordinate
		//q += penOnAxis * u;

		const Float absPen = std::abs(penOnAxis[i]);

		if (absPen < smallPen) {
			smallPen = absPen;
			bestAxis = i;
		}
	}

	q = q + penOnAxis[bestAxis] * getAxis(bestAxis);

	normal = q - point;

	const Float sqrPenetration = glm::length2(normal);

	if (isZero(sqrPenetration)) {
		return false; // we cannot calculate a normal vector for this case!
	}

	penetration = std::sqrt(sqrPenetration);
	normal = normal / penetration;

	return true;
}

}
