#ifndef AYLA_REGULAR_GRID_INDEXER_HPP
#define AYLA_REGULAR_GRID_INDEXER_HPP

#include "ayla/config.hpp"

#include "ayla/geometry/axis_aligned_box.hpp"
#include "ayla/geometry/line_segment.hpp"
#include "ayla/geometry/index_3d.hpp"

#include "ayla/serialization/regular_grid_indexer_serializer.hpp"

#include <vector>

namespace ayla {

/**
 * A simple abstraction to help handle the cells in a regular grid.
 *
 * @remarks This does not store any voxel data. This is a generic
 * abstraction to be used by an actual grid implementation.
 *
 * @see ayla::VoxelGrid
 *
 * @author O Programador
 */
class AYLA_API RegularGridIndexer final {
public:
	/**
	 * Creates an invalid grid indexer.
	 */
	RegularGridIndexer();

	/**
	 * Creates a grid bounded by @param aabb with @param width,
	 * @param height and @param depth cells in each dimension.
	 */
	RegularGridIndexer(const AxisAlignedBox& aabb, SizeType width, SizeType height, SizeType depth);

	/**
	 * @return True if @param index points to a cell that is logically inside this grid.
	 */
	bool contains(const Index3D& index) const;

	/**
	 * @return An axis aligned box containing all the cells in this grid.
	 */
	AxisAlignedBox getAABB() const;

	/**
	 * @return A point that represents the origin of this grid.
	 */
	glm::vec3 getOrigin() const;

	/**
	 * @return The width in number of cells of this grid.
	 */
	SizeType getWidth() const;

	/**
	 * @return The height in number of cells of this grid.
	 */
	SizeType getHeight() const;

	/**
	 * @return The depth in number of cells of this grid.
	 */
	SizeType getDepth() const;

	/**
	 * @return The total number of cells in this grid.
	 */
	SizeType getNumberOfCells() const;

	/**
	 * @return The size of each cell in this grid.
	 */
	glm::vec3 getCellSize() const;

	/**
	 * @return The center of the cell at @param index.
	 */
	glm::vec3 getCellCenter(const Index3D& index) const;

	/**
	 * @return The axis aligned box of the cell at @param index.
	 */
	AxisAlignedBox getCellAABB(const Index3D& index) const;

	/**
	 * @return The index of the cell where @param point lies.
	 */
	Index3D getIndexOfPoint(const glm::vec3& point) const;

	/**
	 * @return The indices of the cells that @param segment intersects.
	 */
	std::vector<Index3D> getCellsOfLineSegment(const LineSegment& segment) const;

	/**
	 * Converts a 3D cell index into an unidimensional value.
	 * This is useful if you store all your grid data in a single unidimensional array.
	 */
	SizeType getOneDimensionalIndex(const Index3D& index) const;

private:
	void _ensureValidSizes();

private:
	AxisAlignedBox _aabb;
	SizeType _width = SizeType(0);
	SizeType _height = SizeType(0);
	SizeType _depth = SizeType(0);

	template<class Archive> friend void boost::serialization::serialize(Archive&, ayla::RegularGridIndexer&, const unsigned int);
};

}

#endif // AYLA_REGULAR_GRID_INDEXER_HPP
