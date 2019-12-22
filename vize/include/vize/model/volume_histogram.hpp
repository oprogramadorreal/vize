#ifndef VIZE_VOLUME_HISTOGRAM_HPP
#define VIZE_VOLUME_HISTOGRAM_HPP

#include "vize/config.hpp"

#include <ayla/geometry/voxel_grid.hpp>

#include <array>

namespace vize {

// @todo: Set this according to volume voxel type
#define NumberOfVoxelValues 256

/**
 * @see vize::Volume
 *
 * @author O Programador
 */
class VolumeHistogram final {
public:
	VolumeHistogram(const ayla::ByteVoxelGrid& volumeData);
	~VolumeHistogram();

public:
	SizeType getMaxVoxelValue() const;

	/**
	 * @return A value between 0 and 1 that indicates how frequent
	 * @param voxelValue appears in the volume data. 0 means that it
	 * never appears and 1 means that it is the value that appears most.
	 */
	Float getNormalizedValueFrequency(SizeType voxelValue) const;

	/**
	 * @return How many times @param voxelValue appears in the volume data.
	 */
	SizeType getValueFrequency(SizeType voxelValue) const;

private:
	const ayla::ByteVoxelGrid& _volumeData;

	std::array<SizeType, NumberOfVoxelValues> _frequencies;
	SizeType _maxFrequencyValue = SizeType(0);
};

}

#endif // VIZE_VOLUME_HISTOGRAM_HPP