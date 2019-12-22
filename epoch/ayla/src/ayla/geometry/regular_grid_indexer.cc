#include "ayla/geometry/regular_grid_indexer.hpp"

namespace ayla {

RegularGridIndexer::RegularGridIndexer() = default;

RegularGridIndexer::RegularGridIndexer(const AxisAlignedBox& aabb, SizeType width, SizeType height, SizeType depth)
	: _aabb(aabb), _width(width), _height(height), _depth(depth)
{
	_ensureValidSizes();
}

void RegularGridIndexer::_ensureValidSizes() {
	_width = (_width == SizeType(0)) ? SizeType(1) : _width;
	_height = (_height == SizeType(0)) ? SizeType(1) : _height;
	_depth = (_depth == SizeType(0)) ? SizeType(1) : _depth;

	_aabb = AxisAlignedBox(
		_aabb.getCenter(),
		isZero(_aabb.getHalfWidth()) ? Float(0.5f) : _aabb.getHalfWidth(),
		isZero(_aabb.getHalfHeight()) ? Float(0.5f) : _aabb.getHalfHeight(),
		isZero(_aabb.getHalfDepth()) ? Float(0.5f) : _aabb.getHalfDepth()
	);
}

Index3D RegularGridIndexer::getIndexOfPoint(const glm::vec3& point) const {
	const auto cellSize = getCellSize();
	const auto pointLocal = point - getOrigin();
	
	return Index3D(
		Index3D::ValueType(std::floor(pointLocal.x / cellSize.x)),
		Index3D::ValueType(std::floor(pointLocal.y / cellSize.y)),
		Index3D::ValueType(std::floor(pointLocal.z / cellSize.z))
	);
}

bool RegularGridIndexer::contains(const Index3D& index) const {
	return
		index.getI() >= Index3D::ValueType(0) && (index.getI() < Index3D::ValueType(getWidth())) &&
		index.getJ() >= Index3D::ValueType(0) && (index.getJ() < Index3D::ValueType(getHeight())) &&
		index.getK() >= Index3D::ValueType(0) && (index.getK() < Index3D::ValueType(getDepth()));
}

AxisAlignedBox RegularGridIndexer::getAABB() const {
	return _aabb;
}

glm::vec3 RegularGridIndexer::getOrigin() const {
	return _aabb.getMinPoint();
}

glm::vec3 RegularGridIndexer::getCellSize() const {
	return _aabb.getExtensions() / glm::vec3(Float(_width), Float(_height), Float(_depth));
}

// Based on "Float-Time Collision Detection", pgs 326-327
std::vector<Index3D> RegularGridIndexer::getCellsOfLineSegment(const LineSegment& segment) const {
	auto indices = std::vector<Index3D>();
	auto clippedSegment = LineSegment();

	if (!_aabb.clipLineSegment(segment, clippedSegment)) {
		return indices;
	}

	const auto p1 = clippedSegment.getP() - getOrigin();
	const auto p2 = clippedSegment.getQ() - getOrigin();

	auto index = Index3D(0);
	auto indexEnd = Index3D(0);
	auto d = Index3D(0);

	std::array<Float, 3> minv, maxv;
	std::array<Float, 3> t;
	std::array<Float, 3> deltat;

	const auto cellSize = getCellSize();

	for (auto i = SizeType(0); i < SizeType(3); ++i) {
		index[i] = Index3D::ValueType(std::floor(p1[i] / cellSize[i]));
		indexEnd[i] = Index3D::ValueType(std::floor(p2[i] / cellSize[i]));

		// determine in which primary direction to step
		d[i] = Index3D::ValueType((p1[i] < p2[i]) ? 1 : ((p1[i] > p2[i]) ? -1 : 0));

		minv[i] = cellSize[i] * std::floor(p1[i] / cellSize[i]);
		maxv[i] = minv[i] + cellSize[i];

		const auto p1p2Abs = Float(std::abs(p2[i] - p1[i]));
		t[i] = ((p1[i] > p2[i]) ? (p1[i] - minv[i]) : (maxv[i] - p1[i])) / p1p2Abs;

		deltat[i] = cellSize[i] / p1p2Abs;
	}

	while (true) {
		if (contains(index)) {
			indices.push_back(index);
		}
		
		// perform step:
		if (t[0] <= t[1] && t[0] <= t[2]) { // t[0] smallest, step in 0
			if (index[0] == indexEnd[0]) break;
			t[0] += deltat[0];
			index[0] += d[0];

		} else if (t[1] <= t[0] && t[1] <= t[2]) { // t[1] smallest, step in 1
			if (index[1] == indexEnd[1]) break;
			t[1] += deltat[1];
			index[1] += d[1];

		} else { // t[2] smallest, step in 2
			if (index[2] == indexEnd[2]) break;
			t[2] += deltat[2];
			index[2] += d[2];
		}
	}

	return indices;
}

glm::vec3 RegularGridIndexer::getCellCenter(const Index3D& index) const {
	const auto cellSize = getCellSize();
	return getOrigin() + (cellSize / 2.0f) + glm::vec3(index.getI() * cellSize.x, index.getJ() * cellSize.y, index.getK() * cellSize.z);
}

AxisAlignedBox RegularGridIndexer::getCellAABB(const Index3D& index) const {
	const auto cellSize = getCellSize();

	return AxisAlignedBox(
		getCellCenter(index),
		Float(cellSize.x / 2.0f),
		Float(cellSize.y / 2.0f),
		Float(cellSize.z / 2.0f)
	);
}

SizeType RegularGridIndexer::getOneDimensionalIndex(const Index3D& index) const {
	assert(contains(index));
	return SizeType(index.getOneDimensionalIndex(getWidth(), getHeight()));
}

SizeType RegularGridIndexer::getWidth() const {
	return _width;
}

SizeType RegularGridIndexer::getHeight() const {
	return _height;
}

SizeType RegularGridIndexer::getDepth() const {
	return _depth;
}

SizeType RegularGridIndexer::getNumberOfCells() const {
	return getWidth() * getHeight() * getDepth();
}

}
