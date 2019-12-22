#include "vize/model/volume.hpp"
#include "vize/model/volume_histogram.hpp"

namespace vize {

Volume::Volume() = default;

Volume::Volume(SizeType widthInVolxes, SizeType heightInVoxels, SizeType depthInVoxels)
	: _voxelGrid(widthInVolxes, heightInVoxels, depthInVoxels)
{ }

Volume::~Volume() = default;

std::unique_ptr<Volume> Volume::extractSubVolume(const ayla::Index3D& subVolumeBegin, SizeType subVolumeWidth, SizeType subVolumeHeight, SizeType subVolumeDepth) const {
	auto subVolume = std::make_unique<Volume>(subVolumeWidth, subVolumeHeight, subVolumeDepth);
	subVolume->_modelMatrix = _modelMatrix;

	for (auto i = SizeType(0); i < subVolume->getWidth(); ++i) {
		for (auto j = SizeType(0); j < subVolume->getHeight(); ++j) {
			for (auto k = SizeType(0); k < subVolume->getDepth(); ++k) {
				const auto subVolumeIndex = ayla::Index3D(i, j, k);
				const auto volumeIndex = subVolumeBegin + subVolumeIndex;

				auto voxelValue = std::uint8_t(0);

				if (_voxelGrid.contains(volumeIndex)) {
					voxelValue = _voxelGrid.getVoxel(volumeIndex);
				}

				subVolume->_voxelGrid.setVoxel(subVolumeIndex, voxelValue);
			}
		}
	}

	return std::move(subVolume);
}

SizeType Volume::getWidth() const { return _voxelGrid.getWidth(); }
SizeType Volume::getHeight() const { return _voxelGrid.getHeight(); }
SizeType Volume::getDepth() const { return _voxelGrid.getDepth(); }
SizeType Volume::getNumberOfVoxels() const { return _voxelGrid.getNumberOfVoxels(); }

const std::uint8_t* Volume::getRawData() const { return _voxelGrid.getRawVoxelData(); }

glm::mat4 Volume::getModelMatrix() const { return _modelMatrix; }

void Volume::setModelMatrix(const glm::mat4& modelMatrix) {
	_modelMatrix = modelMatrix;
}

ayla::AxisAlignedBox Volume::getAABB() const {
	const auto aabb = _voxelGrid.getAABB();

	// TODO: fix here... surely this is not correct:
	const auto a = glm::vec3(_modelMatrix * glm::vec4(aabb.getMinPoint(), 1.0f));
	const auto b = glm::vec3(_modelMatrix * glm::vec4(aabb.getMaxPoint(), 1.0f));

	const auto minPoint = glm::vec3(
		std::min(a.x, b.x),
		std::min(a.y, b.y),
		std::min(a.z, b.z)
	);
	const auto maxPoint = glm::vec3(
		std::max(a.x, b.x),
		std::max(a.y, b.y),
		std::max(a.z, b.z)
	);

	return ayla::AxisAlignedBox(minPoint, maxPoint);
}

const VolumeHistogram* Volume::getHistogram() {
	if (!_histogram) {
		_histogram = std::make_unique<VolumeHistogram>(_voxelGrid);
	}

	return _histogram.get();
}

/**
 * Based on: http://www.songho.ca/dsp/histogram/histogram.html
 */
void Volume::applyHistogramEqualization() {
	const auto histogram = getHistogram();

	if (!histogram) {
		return;
	}

	const auto volumeSize = getNumberOfVoxels();
	const auto scale = (histogram->getMaxVoxelValue() - 1) / Float(volumeSize);
	
	std::array<SizeType, NumberOfVoxelValues> lut;
	
	auto sum = SizeType(0);
	
	for (auto i = SizeType(0); i < lut.size(); ++i) {
		sum += histogram->getValueFrequency(i);
		lut[i] = SizeType(sum * scale);
	}
	
	for (auto i = SizeType(0); i < getNumberOfVoxels(); ++i) {
		_voxelGrid.setVoxel(i, lut[_voxelGrid.getVoxel(i)]);
	}

	_histogram.reset();
}

}