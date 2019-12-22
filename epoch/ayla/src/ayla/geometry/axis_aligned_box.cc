#include "ayla/geometry/axis_aligned_box.hpp"

#include "ayla/geometry/vector.hpp"
#include "ayla/geometry/plane.hpp"
#include "ayla/geometry/intersection.hpp"

#include <glm/gtx/matrix_decompose.hpp>

namespace ayla {

AxisAlignedBox::AxisAlignedBox() = default;

AxisAlignedBox::AxisAlignedBox(const AxisAlignedBox& other)
	: _minPt(other._minPt), _maxPt(other._maxPt)
{ }

AxisAlignedBox::AxisAlignedBox(const std::vector<glm::vec3>& setOfPoints) 
	: _minPt(std::numeric_limits<Float>::max()), _maxPt(-std::numeric_limits<Float>::max())
{
	for (SizeType i = 0u; i < setOfPoints.size(); ++i) {
		auto each = setOfPoints[i];

		for (SizeType axis = 0u; axis < 3u; ++axis) {
			if (each[axis] < _minPt[axis]) {
				_minPt[axis] = each[axis];
			}

			if (each[axis] > _maxPt[axis]) {
				_maxPt[axis] = each[axis];
			}
		}
	}
}

AxisAlignedBox::AxisAlignedBox(const std::vector<glm::vec2>& setOfPoints)
	: _minPt(std::numeric_limits<Float>::max()), _maxPt(-std::numeric_limits<Float>::max())
{
	for (SizeType i = 0u; i < setOfPoints.size(); ++i) {
		auto each = setOfPoints[i];

		for (SizeType axis = 0u; axis < 2u; ++axis) {
			if (each[axis] < _minPt[axis]) {
				_minPt[axis] = each[axis];
			}

			if (each[axis] > _maxPt[axis]) {
				_maxPt[axis] = each[axis];
			}
		}
	}

	_maxPt.z = _minPt.z = 0.0f;
}

AxisAlignedBox::AxisAlignedBox(const glm::vec3& center, const Float& halfWidth, const Float& halfHeight, const Float& halfDepth)
	: _minPt(center.x - halfWidth, center.y - halfHeight, center.z - halfDepth), _maxPt(center.x + halfWidth, center.y + halfHeight, center.z + halfDepth)
{
	assert(halfWidth >= 0.0f);
	assert(halfHeight >= 0.0f);
	assert(halfDepth >= 0.0f);
}

AxisAlignedBox::AxisAlignedBox(const glm::vec3& minPt, const glm::vec3& maxPt)
	: _minPt(minPt), _maxPt(maxPt)
{
	assert(_minPt.x <= maxPt.x);
	assert(_minPt.y <= maxPt.y);
	assert(_minPt.z <= maxPt.z);
}

AxisAlignedBox::AxisAlignedBox(const AxisAlignedBox& one, const AxisAlignedBox& two) {
	AxisAlignedBox::merge(one, two, *this);
}

AxisAlignedBox AxisAlignedBox::merge(const AxisAlignedBox& a, const AxisAlignedBox& b) {
	AxisAlignedBox r;
	AxisAlignedBox::merge(a, b, r);
	return r;
}

void AxisAlignedBox::merge(const AxisAlignedBox& a, const AxisAlignedBox& b, AxisAlignedBox& result) {
	for (SizeType i = 0u; i < 3u; ++i) {
		result._minPt[i] = (a._minPt[i] < b._minPt[i]) ? a._minPt[i] : b._minPt[i];
		result._maxPt[i] = (a._maxPt[i] > b._maxPt[i]) ? a._maxPt[i] : b._maxPt[i];
	}
}

glm::vec3 AxisAlignedBox::getCenter() const {
	return (_minPt + _maxPt)*0.5f;
}

void AxisAlignedBox::setCenter(const glm::vec3& center) {
	const Float hW = getHalfWidth();
	const Float hH = getHalfHeight();
	const Float hD = getHalfDepth();

	_minPt.x = (center.x-hW);
	_minPt.y = (center.y-hH);
	_minPt.z = (center.z-hD);

	_maxPt.x = (center.x+hW);
	_maxPt.y = (center.y+hH);
	_maxPt.z = (center.z+hD);
}

Float AxisAlignedBox::getWidth() const {
	return _maxPt.x - _minPt.x;
}

Float AxisAlignedBox::getHeight() const {
	return _maxPt.y - _minPt.y;
}

Float AxisAlignedBox::getDepth() const {
	return _maxPt.z - _minPt.z;
}

Float AxisAlignedBox::getHalfWidth() const {
	return getWidth()*0.5f;
}

Float AxisAlignedBox::getHalfHeight() const {
	return getHeight()*0.5f;
}

Float AxisAlignedBox::getHalfDepth() const {
	return getDepth()*0.5f;
}

void AxisAlignedBox::translate(const glm::vec3& t) {
	_minPt += t;
	_maxPt += t;
}

AxisAlignedBox AxisAlignedBox::translated(const glm::vec3& t) const {
	AxisAlignedBox cpy(*this);
	cpy.translate(t);
	return cpy;
}

void AxisAlignedBox::transform(const glm::vec3& translation, const glm::mat3& rotation) {
	// Real-time Collision Detection pags 86-87

	glm::vec3 newMinPt, newMaxPt;

	for (SizeType i = 0u; i < 3u; ++i) { // for all three axes
		newMinPt[i] = newMaxPt[i] = translation[i]; // starting by adding in translation

		// form extent by summing smaller and larger terms respectively
		for (SizeType j = 0u; j < 3u; ++j) {
			const Float e = rotation[i][j] * _minPt[j];
			const Float f = rotation[i][j] * _maxPt[j];

			if (e < f) {
				newMinPt[i] += e;
				newMaxPt[i] += f;
			} else {
				newMinPt[i] += f;
				newMaxPt[i] += e;
			}
		}
	}

	_minPt = newMinPt;
	_maxPt = newMaxPt;
}

bool AxisAlignedBox::transform(const glm::mat4& modelMatrix) {
	glm::vec3 scale;
	glm::quat orientation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;

	if (!glm::decompose(modelMatrix, scale, orientation, translation, skew, perspective)) {
		return false;
	}

	expand(scale);
	transform(translation, glm::toMat3(orientation));

	return true;
}

AxisAlignedBox AxisAlignedBox::transformed(const glm::vec3& translation, const glm::mat3& rotation) const {
	AxisAlignedBox cpy(*this);
	cpy.transform(translation, rotation);
	return cpy;
}

glm::vec3 AxisAlignedBox::getHalfExtensions() const {
	return glm::vec3(getHalfWidth(), getHalfHeight(), getHalfDepth());
}

glm::vec3 AxisAlignedBox::getExtensions() const {
	return glm::vec3(getWidth(), getHeight(), getDepth());
}

glm::vec3 AxisAlignedBox::getMaxPoint() const {
	return _maxPt;
}

glm::vec3 AxisAlignedBox::getMinPoint() const {
	return _minPt;
}

void AxisAlignedBox::getAABBLimits(glm::vec3& minPt, glm::vec3& maxPt) const {
	minPt = getMinPoint();
	maxPt = getMaxPoint();
}

Float AxisAlignedBox::getVolume() const {
	return getWidth()*getHeight()*getDepth();
}

glm::vec3 AxisAlignedBox::getClosestPoint(const glm::vec3& pt) const {
	// Real-Time Collision Detection pg 130

	// For each coordinate axis, if the point coordinate value is
	// outside box, clamp it to the box, else keep it as is
	glm::vec3 r;

	for (int i = 0; i < 3; i++) {
		Float v = pt[i];
		if (v < _minPt[i]) v = _minPt[i]; // v = max(v, b.min[i])
		if (v > _maxPt[i]) v = _maxPt[i]; // v = min(v, b.max[i])
		r[i] = v;
	}

	return r;
}

Float AxisAlignedBox::getSqrDistance(const glm::vec3 &pt) const {
	// Real-Time Collision Detection pgs 131-132

	Float sqDist(0.0f);

	for (int i = 0; i < 3; i++) {
		// For each axis count any excess distance outside box extents
		Float v = pt[i];
		if (v < _minPt[i]) sqDist += (_minPt[i] - v) * (_minPt[i] - v);
		if (v > _maxPt[i]) sqDist += (v - _maxPt[i]) * (v - _maxPt[i]);
	}

	return sqDist;
}

void AxisAlignedBox::getDirExtremePts(const glm::vec3& normDir, glm::vec3& posDirPt, glm::vec3& negDirPt) const {
	for (SizeType i = 0u; i < 3u; ++i) {
		if (normDir[i] < 0.0f) {
			posDirPt[i] = _minPt[i];
			negDirPt[i] = _maxPt[i];
		} else {
			posDirPt[i] = _maxPt[i];
			negDirPt[i] = _minPt[i];
		}
	}
}

bool AxisAlignedBox::intersects(const AxisAlignedBox& other) const {
	return (_minPt.x <= other._maxPt.x) &&
		(_maxPt.x >= other._minPt.x) &&
		(_minPt.y <= other._maxPt.y) &&
		(_maxPt.y >= other._minPt.y) &&
		(_minPt.z <= other._maxPt.z) &&
		(_maxPt.z >= other._minPt.z);
}

bool AxisAlignedBox::contains(const glm::vec3& pt) const {
	return (pt.x >= _minPt.x) && (pt.x <= _maxPt.x) &&
		(pt.y >= _minPt.y) && (pt.y <= _maxPt.y) &&
		(pt.z >= _minPt.z) && (pt.z <= _maxPt.z);
}

bool AxisAlignedBox::contains(const AxisAlignedBox& other) const {
	return (
		(_minPt.x <= other._minPt.x) &&
		(_minPt.y <= other._minPt.y) &&
		(_minPt.z <= other._minPt.z) &&
		(_maxPt.x >= other._maxPt.x) &&
		(_maxPt.y >= other._maxPt.y) &&
		(_maxPt.z >= other._maxPt.z)
	);
}

void AxisAlignedBox::expand(const glm::vec3& e) {
	_minPt -= e;
	_maxPt += e;
}

void AxisAlignedBox::signedExpand(const glm::vec3& e) {
	for (SizeType i = 0u; i < 3u; ++i) {
		if (e[i] > 0.0f) {
			_maxPt[i] += e[i];
		} else {
			_minPt[i] += e[i];
		}
	}
}

bool AxisAlignedBox::notEqual(const AxisAlignedBox& other) const {
	return (
		(_minPt.x != other._minPt.x) ||
		(_minPt.y != other._minPt.y) ||
		(_minPt.z != other._minPt.z) ||
		(_maxPt.x != other._maxPt.x) ||
		(_maxPt.y != other._maxPt.y) ||
		(_maxPt.z != other._maxPt.z)
	);
}

glm::vec3 AxisAlignedBox::getVertex(SizeType index) const {
	assert(index < 8u);

	const static glm::vec3 signals[8] = {
		glm::vec3( -1, -1, -1 ),
		glm::vec3( -1, -1,  1 ),
		glm::vec3( -1,  1, -1 ),
		glm::vec3( -1,  1,  1 ),
		glm::vec3(  1, -1, -1 ),
		glm::vec3(  1, -1,  1 ),
		glm::vec3(  1,  1, -1 ),
		glm::vec3(  1,  1,  1 )
	};

	return getCenter() + (getHalfExtensions() * signals[index]);
}

std::array<glm::vec3, 8> AxisAlignedBox::getVertices() const {
	std::array<glm::vec3, 8> vertices;
	vertices[0] = _minPt;
	vertices[7] = _maxPt;

	for (SizeType i = 1u; i < 7u; ++i) {
		vertices[i] = getVertex(i);
	}

	return vertices;
}

std::array<LineSegment, 12> AxisAlignedBox::getEdges() const {
	std::array<glm::vec3, 8> vertices = getVertices();

	std::array<LineSegment, 12> edges;
	edges[0]  = LineSegment(vertices[0], vertices[1]);
	edges[1]  = LineSegment(vertices[2], vertices[3]);
	edges[2]  = LineSegment(vertices[6], vertices[7]);
	edges[3]  = LineSegment(vertices[4], vertices[5]);
	edges[4]  = LineSegment(vertices[1], vertices[5]);
	edges[5]  = LineSegment(vertices[3], vertices[7]);
	edges[6]  = LineSegment(vertices[2], vertices[6]);
	edges[7]  = LineSegment(vertices[0], vertices[4]);
	edges[8]  = LineSegment(vertices[0], vertices[2]);
	edges[9]  = LineSegment(vertices[1], vertices[3]);
	edges[10] = LineSegment(vertices[5], vertices[7]);
	edges[11] = LineSegment(vertices[4], vertices[6]);

	return edges;
}

bool AxisAlignedBox::clipLineSegment(const LineSegment& segment, LineSegment& clipedSegment) const {
	const Ray pqRay(segment.getP(), glm::normalize(segment.getPQ()) );
	Float pqDistance;

	if (!intersection::rayAab(pqRay, *this, pqDistance)) {
		return false;
	}

	const Ray qpRay(segment.getQ(), glm::normalize(segment.getQP()) );
	Float qpDistance;

	if (!intersection::rayAab(qpRay, *this, qpDistance)) {
		return false;
	}

	clipedSegment = LineSegment(pqRay.getPointAt(pqDistance), qpRay.getPointAt(qpDistance));

	return true;
}

glm::vec3 AxisAlignedBox::relativePosition(const glm::vec3& point) const {
	return (point - getMinPoint()) / getExtensions();
};

}
