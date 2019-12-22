#include "ayla/geometry/voxel_grid.hpp"

#include "ayla/interpolation.hpp"

namespace ayla {

template <class T>
VoxelGrid<T>::VoxelGrid() = default;

template <class T>
VoxelGrid<T>::VoxelGrid(SizeType width, SizeType height, SizeType depth)
	: VoxelGrid(width, height, depth, AxisAlignedBox(glm::vec3(0.0f), Float(0.5f), Float(0.5f), Float(0.5f)))
{ }

template <class T>
VoxelGrid<T>::VoxelGrid(SizeType width, SizeType height, SizeType depth, const AxisAlignedBox& aabb)
	: _cellsIndexer(aabb, SizeType(width - 1), SizeType(height - 1), SizeType(depth - 1))
{
	assert(width > SizeType(1));
	assert(height > SizeType(1));
	assert(depth > SizeType(1));

	const auto numberOfVoxels = width * height * depth;
	_voxels.resize(numberOfVoxels, T());
}

template <class T>
SizeType VoxelGrid<T>::getWidth() const {
	return _cellsIndexer.getWidth() + SizeType(1);
}

template <class T>
SizeType VoxelGrid<T>::getHeight() const {
	return _cellsIndexer.getHeight() + SizeType(1);
}

template <class T>
SizeType VoxelGrid<T>::getDepth() const {
	return _cellsIndexer.getDepth() + SizeType(1);
}

template <class T>
SizeType VoxelGrid<T>::getNumberOfVoxels() const {
	return SizeType(_voxels.size());
}

template <class T>
T VoxelGrid<T>::getVoxel(SizeType voxelIndex) const {
	assert(voxelIndex < getNumberOfVoxels());
	return _voxels[voxelIndex];
}

template <class T>
void VoxelGrid<T>::setVoxel(const SizeType& voxelIndex, const T& voxelValue) {
	assert(voxelIndex < getNumberOfVoxels());
	_voxels[voxelIndex] = voxelValue;
}

template <class T>
SizeType VoxelGrid<T>::_getIndexInVoxelsArray(const Index3D& index) const {
	return SizeType(
		index.getOneDimensionalIndex(getWidth(), getHeight())
	);
}

template <class T>
T VoxelGrid<T>::getVoxel(const Index3D& voxelIndex) const {
	assert(contains(voxelIndex));
	const auto index1D = _getIndexInVoxelsArray(voxelIndex);
	assert(index1D < getNumberOfVoxels());
	return _voxels[index1D];
}

template <class T>
void VoxelGrid<T>::setVoxel(const Index3D& voxelIndex, const T& voxelValue) {
	assert(contains(voxelIndex));
	const auto index1D = _getIndexInVoxelsArray(voxelIndex);
	assert(index1D < getNumberOfVoxels());
	_voxels[index1D] = voxelValue;
}

template <class T>
glm::vec3 VoxelGrid<T>::getVoxelPosition(const Index3D& voxelIndex) const {
	assert(contains(voxelIndex));
	return _cellsIndexer.getCellAABB(voxelIndex).getMinPoint();
}

template <class T>
const T* VoxelGrid<T>::getRawVoxelData() const {
	return _voxels.data();
}

template <class T> 
T VoxelGrid<T>::_interpolatedVoxelValue(const glm::vec3& relativePointInCell, const CellVoxels& voxels) const {
	// http://en.wikipedia.org/wiki/Trilinear_interpolation

	const auto x = Float(glm::clamp(relativePointInCell.x, 0.0f, 1.0f));
	const auto y = Float(glm::clamp(relativePointInCell.y, 0.0f, 1.0f));
	const auto z = Float(glm::clamp(relativePointInCell.z, 0.0f, 1.0f));

	// x interpolation:
	const auto c00 = linearInterpolation(voxels[0], voxels[4], x);
	const auto c10 = linearInterpolation(voxels[2], voxels[6], x);
	const auto c01 = linearInterpolation(voxels[1], voxels[5], x);
	const auto c11 = linearInterpolation(voxels[3], voxels[7], x);

	// y interpolation:
	const auto c0 = linearInterpolation(c00, c10, y);
	const auto c1 = linearInterpolation(c01, c11, y);

	// z interpolation:
	return linearInterpolation(c0, c1, z);
}

template <class T>
T VoxelGrid<T>::sample(const glm::vec3& point) const {
	const auto cellIndex = _cellsIndexer.getIndexOfPoint(point);

	if (!_cellsIndexer.contains(cellIndex)) {
		return T();
	}

	return _interpolatedVoxelValue(
		getCellAABB(cellIndex).relativePosition(point),
		getCellVoxels(cellIndex)
	);
}

template <class T>
glm::vec3 VoxelGrid<T>::sampleGradient(const glm::vec3& point) const {
	const auto cellIndex = _cellsIndexer.getIndexOfPoint(point);

	if (!_cellsIndexer.contains(cellIndex)) {
		return glm::vec3(0.0f);
	}

	const auto cellAABB = getCellAABB(cellIndex);
	const auto voxels = getCellVoxels(cellIndex);

	const auto relPoint = cellAABB.relativePosition(point);

	return glm::vec3(
		_interpolatedVoxelValue(glm::vec3(1.0f, relPoint.y, relPoint.z), voxels) - _interpolatedVoxelValue(glm::vec3(0.0f, relPoint.y, relPoint.z), voxels),
		_interpolatedVoxelValue(glm::vec3(relPoint.x, 1.0f, relPoint.z), voxels) - _interpolatedVoxelValue(glm::vec3(relPoint.x, 0.0f, relPoint.z), voxels),
		_interpolatedVoxelValue(glm::vec3(relPoint.x, relPoint.y, 1.0f), voxels) - _interpolatedVoxelValue(glm::vec3(relPoint.x, relPoint.y, 0.0f), voxels)
	);
}

template <class T>
SizeType VoxelGrid<T>::getWidthInCells() const {
	return _cellsIndexer.getWidth();
}

template <class T>
SizeType VoxelGrid<T>::getHeightInCells() const {
	return _cellsIndexer.getHeight();
}

template <class T>
SizeType VoxelGrid<T>::getDepthInCells() const {
	return _cellsIndexer.getDepth();
}

template <class T>
typename VoxelGrid<T>::CellVoxels VoxelGrid<T>::getCellVoxels(const Index3D& cellIndex) const {
	assert(containsCell(cellIndex));

	const auto i = cellIndex.getI();
	const auto j = cellIndex.getJ();
	const auto k = cellIndex.getK();

	CellVoxels voxels;
	voxels[0] = getVoxel(Index3D(i, j, k));
	voxels[1] = getVoxel(Index3D(i, j, k + 1));
	voxels[2] = getVoxel(Index3D(i, j + 1, k));
	voxels[3] = getVoxel(Index3D(i, j + 1, k + 1));
	voxels[4] = getVoxel(Index3D(i + 1, j, k));
	voxels[5] = getVoxel(Index3D(i + 1, j, k + 1));
	voxels[6] = getVoxel(Index3D(i + 1, j + 1, k));
	voxels[7] = getVoxel(Index3D(i + 1, j + 1, k + 1));

	return voxels;
}

template <class T>
AxisAlignedBox VoxelGrid<T>::getCellAABB(const Index3D& cellIndex) const {
	return _cellsIndexer.getCellAABB(cellIndex);
}

template <class T>
Index3D VoxelGrid<T>::getCellIndexOfPoint(const glm::vec3& point) const {
	return _cellsIndexer.getIndexOfPoint(point);
}

template <class T>
AxisAlignedBox VoxelGrid<T>::getAABB() const {
	return _cellsIndexer.getAABB();
}

template <class T>
bool VoxelGrid<T>::containsCell(const Index3D& cellIndex) const {
	return _cellsIndexer.contains(cellIndex);
}

template <class T>
bool VoxelGrid<T>::contains(const Index3D& voxelIndex) const {
	return
		voxelIndex.getI() >= Index3D::ValueType(0) && (voxelIndex.getI() < Index3D::ValueType(getWidth())) &&
		voxelIndex.getJ() >= Index3D::ValueType(0) && (voxelIndex.getJ() < Index3D::ValueType(getHeight())) &&
		voxelIndex.getK() >= Index3D::ValueType(0) && (voxelIndex.getK() < Index3D::ValueType(getDepth()));
}

template <class T>
bool VoxelGrid<T>::contains(const glm::vec3& point) const {
	return containsCell(getCellIndexOfPoint(point));
}

template class VoxelGrid<Float>;
template class VoxelGrid<std::uint8_t>;

}