#include "vize/model/volume_histogram.hpp"

namespace vize {

VolumeHistogram::VolumeHistogram(const ayla::ByteVoxelGrid& volumeData)
	: _volumeData(volumeData)
{
	_frequencies.fill(SizeType(0));

	for (auto i = SizeType(0); i < volumeData.getNumberOfVoxels(); ++i) {
		const auto value = volumeData.getVoxel(i);
		assert(value < _frequencies.size());
		++_frequencies[value];
	}

	auto maxOccurence = SizeType(0);
	
	for (const auto occurence : _frequencies) {
		if (occurence > maxOccurence) {
			maxOccurence = occurence;
		}
	}
	
	_maxFrequencyValue = maxOccurence;
}

VolumeHistogram::~VolumeHistogram() = default;

SizeType VolumeHistogram::getMaxVoxelValue() const {
	return SizeType(_frequencies.size());
}

Float VolumeHistogram::getNormalizedValueFrequency(SizeType voxelValue) const {
	assert(_maxFrequencyValue > SizeType(0));
	return getValueFrequency(voxelValue) / Float(_maxFrequencyValue);
}

SizeType VolumeHistogram::getValueFrequency(SizeType voxelValue) const {
	assert(voxelValue < getMaxVoxelValue());
	return _frequencies[voxelValue];
}

}