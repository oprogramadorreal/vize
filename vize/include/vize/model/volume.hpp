#ifndef VIZE_VOLUME_HPP
#define VIZE_VOLUME_HPP

#include "vize/config.hpp"

#include "vize/serialization/volume_serializer.hpp"

#include <ayla/geometry/voxel_grid.hpp>

namespace vize {

class VolumeHistogram;

/**
 * A tridimensional array of pixels (or "voxels"). Or a pile of images.
 *
 * A volume can be loaded from a sequence of image files.
 *
 * Each voxel of this volume is supposed to be stored in only one byte.
 * The voxel data can be seen as an intensity value ranging from 0 to 255.
 *
 * The volume abstraction will be used to create an OpenGL 3D texture,
 * where will be assumed that each pixel of the 3D texture is stored in
 * only one byte.
 *
 * @see vize::VolumeFactory
 * @see vize::GLVolumeTexture
 *
 * @author O Programador
 */
class Volume final {
public:
	Volume();
	Volume(SizeType widthInVolxes, SizeType heightInVoxels, SizeType depthInVoxels);

	~Volume();

public:
	/**
	 * Extract a sub volume of this volume starting at @param subVolumeBegin and
	 * with dimensions specified by @param subVolumeWidth, @param subVolumeHeight and @param subVolumeDepth.
	 */
	std::unique_ptr<Volume> extractSubVolume(const ayla::Index3D& subVolumeBegin, SizeType subVolumeWidth, SizeType subVolumeHeight, SizeType subVolumeDepth) const;

	/**
	 * @return The width of this volume in voxels.
	 */
	SizeType getWidth() const;

	/**
	 * @return The height of this volume in voxels.
	 */
	SizeType getHeight() const;

	/**
	 * @return The depth of this volume in voxels.
	 */
	SizeType getDepth() const;

	/**
	 * @return The total number of voxels stored by this volume.
	 */
	SizeType getNumberOfVoxels() const;

	/**
	 * @return A pointer to the first byte of this volume data.
	 * @remarks Volume data is stored in an internal array.
	 */
	const std::uint8_t* getRawData() const;

	/**
	 * A model matrix is used to give position, orientation and size to the volume.
	 * Use this matrix to place a volume in a 3D space.
	 *
	 * @return The model matrix of this volume.
	 */
	glm::mat4 getModelMatrix() const;

	/**
	 * Set the model matrix of this volume.
	 */
	void setModelMatrix(const glm::mat4& modelMatrix);

	/**
	 * @return A bounding box for this volume.
	 */
	ayla::AxisAlignedBox getAABB() const;

	const VolumeHistogram* getHistogram();

	void applyHistogramEqualization();

private:
	ayla::ByteVoxelGrid _voxelGrid;
	glm::mat4 _modelMatrix = glm::mat4(1.0f); // gives position, orientation and size to the volume
	std::unique_ptr<VolumeHistogram> _histogram;

private:
	friend class TiffVolumeFactory;
	friend class DicomVolumeFactory;
	friend class VolumeFactory;

	template<class Archive> friend void boost::serialization::serialize(Archive&, vize::Volume&, const unsigned int);
	template<class Archive> friend void boost::serialization::load_construct_data(Archive&, vize::Volume*, const unsigned int);
};

}

#endif // VIZE_VOLUME_HPP