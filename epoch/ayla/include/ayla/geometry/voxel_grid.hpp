#ifndef AYLA_VOXEL_GRID_HPP
#define AYLA_VOXEL_GRID_HPP

#include "ayla/config.hpp"

#include "ayla/geometry/regular_grid_indexer.hpp"

#include "ayla/serialization/voxel_grid_serializer.hpp"

namespace ayla {

/**
 * A regular grid of "voxels" (aka "3D pixel").
 *
 * This can be seen as a "volumetric image".
 * According to Wikipedia: "A voxel represents a value on a regular grid in three-dimensional space."
 *
 * @see https://en.wikipedia.org/wiki/Voxel
 * @see ayla::TriangleMeshDistanceField
 *
 * @author O Programador
 */
template <class T>
class AYLA_API VoxelGrid final {
public:
	/**
	 * Creates an empty voxel grid.
	 */
	VoxelGrid();

	/**
	 * Creates a voxel grid with @param width, @param height
	 * and @param depth voxels in each dimension.
	 *
	 * @remarks The grid AABB will be a cube with unit length edges centered at (0,0,0).
	 */
	VoxelGrid(SizeType width, SizeType height, SizeType depth);

	/**
	 * Creates a grid bounded by @param aabb with @param width,
	 * @param height and @param depth voxels in each dimension.
	 */
	VoxelGrid(SizeType width, SizeType height, SizeType depth, const AxisAlignedBox& aabb);

	/**
	 * @return The width of this grid in number of voxels.
	 */
	SizeType getWidth() const;

	/**
	 * @return The height of this grid in number of voxels.
	 */
	SizeType getHeight() const;

	/**
	 * @return The depth of this grid in number of voxels.
	 */
	SizeType getDepth() const;

	/**
	 * @return The total number of voxels stored by this grid.
	 *
	 * @remarks This must be equal to: getWidth() * getHeight() * getDepth()
	 */
	SizeType getNumberOfVoxels() const;

	/**
	 * Provides direct access to voxels array data.
	 *
	 * @param voxelIndex must be greater or equal to zero and less than value returned by "getNumberOfVoxels" method.
	 * @return The voxel value at @param voxelIndex position on the internal array.
	 */
	T getVoxel(SizeType voxelIndex) const;

	/**
	 * Provides direct access to voxels array data.
	 *
	 * Set the value of the voxel specified by @param voxelIndex with @param voxelValue.
	 */
	void setVoxel(const SizeType& voxelIndex, const T& voxelValue);

	/**
	 * @return The value of the voxel at @param voxelIndex in this grid.
	 */
	T getVoxel(const Index3D& voxelIndex) const;

	/**
	 * Set the value of the voxel specified by @param voxelIndex with @param voxelValue.
	 */
	void setVoxel(const Index3D& voxelIndex, const T& voxelValue);

	/**
	 * @return A position in 3D space for the voxel specified by @param voxelIndex.
	 *
	 * @remarks The returned position is inside the AABB of this grid.
	 */
	glm::vec3 getVoxelPosition(const Index3D& voxelIndex) const;

	/**
	 * @return A pointer to the first voxel data of this grid.
	 *
	 * @remarks Voxels are stored sequentially in memory using an internal array structure.
	 * The size of the internal array is returned by "getNumberOfVoxels" method.
	 */
	const T* getRawVoxelData() const;

	/**
	 * @return An interpolated voxel value at position specified by @param point.
	 *
	 * @remarks @param point must be inside this grid, otherwise the returned value is invalid.
	 * Use the method called "contains" to verify if @param point is inside this grid.
	 */
	T sample(const glm::vec3& point) const;

	/**
	 * @return ...
	 *
	 * @remarks @param point must be inside this grid, otherwise the returned value is invalid.
	 * Use the method called "contains" to verify if @param point is inside this grid.
	 */
	glm::vec3 sampleGradient(const glm::vec3& point) const;

	/**
	 * @return The width of this grid in number of cells.
	 */
	SizeType getWidthInCells() const;

	/**
	 * @return The height of this grid in number of cells.
	 */
	SizeType getHeightInCells() const;

	/**
	 * @return The depth of this grid in number of cells.
	 */
	SizeType getDepthInCells() const;

	/**
	 * One voxel value for each vertex of the grid cell.
	 */
	using CellVoxels = std::array<T, 8u>;

	/**
	 * @return One cell (8 voxels) of this grid.
	 */
	CellVoxels getCellVoxels(const Index3D& cellIndex) const;

	/**
	 * @return The AABB of the cell specified by @param cellIndex.
	 */
	AxisAlignedBox getCellAABB(const Index3D& cellIndex) const;

	/**
	 * @return The index of the cell where @param point lies.
	 */
	Index3D getCellIndexOfPoint(const glm::vec3& point) const;

	/**
	 * @return A bounding box for this grid.
	 */
	AxisAlignedBox getAABB() const;

	/**
	 * @return True if @param cellIndex points to a cell that is logically inside this grid.
	 */
	bool containsCell(const Index3D& cellIndex) const;

	/**
	 * @return True if @param voxelIndex points to a voxel that is logically inside this grid.
	 */
	bool contains(const Index3D& voxelIndex) const;

	/**
	 * @return True if @param point is inside the AABB of this grid.
	 */
	bool contains(const glm::vec3& point) const;

private:
	/**
	 * Converts @param index from a 3D index to a 1D index to be used in "_voxels" vector.
	 */
	SizeType _getIndexInVoxelsArray(const Index3D& index) const;

	/**
	 * Implements trilinear interpolation based on:
	 * http://en.wikipedia.org/wiki/Trilinear_interpolation
	 */
	T _interpolatedVoxelValue(const glm::vec3& relativePointInCell, const CellVoxels& voxels) const;

private:
	RegularGridIndexer _cellsIndexer;
	std::vector<T> _voxels;

	template <class Archive, class U> friend void boost::serialization::serialize(Archive&, ayla::VoxelGrid<U>&, const unsigned int);
};

using FloatVoxelGrid = VoxelGrid<Float>;
using ByteVoxelGrid = VoxelGrid<std::uint8_t>;

}

#endif // AYLA_VOXEL_GRID_HPP