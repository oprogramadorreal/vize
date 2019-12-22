#include "ayla/geometry/triangle_mesh_distance_field_builder.hpp"

#include "ayla/geometry/intersection.hpp"

namespace ayla {

namespace {

bool _isPointInsideMesh(const glm::vec3& point, const TriangleMesh& mesh, Float farDistance) {
	const auto _isInside = [&point, &mesh](const glm::vec3& farPoint) -> bool {
		auto counter = SizeType(0);

		for (auto f = SizeType(0); f < mesh.getNumberOfFaces(); ++f) {
			const auto triangle = mesh.getFaceTriangle(f);
			const auto segment = LineSegment(point, farPoint);

			if (intersection::triangleLineSegment(triangle, segment)) {
				++counter;
			}
		}

		return (counter % 2) != 0; // if number of intersections is odd, point is inside
	};

	auto inCount = SizeType(0);

	const std::array<glm::vec3, 5u> farPoints = {
		glm::vec3(farDistance, 0, 0),
		glm::vec3(0, farDistance, 0),
		glm::vec3(0, 0, farDistance),
		glm::vec3(farDistance, 0, farDistance),
		glm::vec3(-farDistance, farDistance, 0)
	};

	// test different directions, just to make sure
	for (auto i = SizeType(0); i < farPoints.size() && (inCount < SizeType(3)); ++i) {
		if (_isInside(farPoints[i])) {
			++inCount;
		}
	}

	return inCount >= SizeType(3);
}

Float _calculateDistance(const glm::vec3& point, const TriangleMesh& mesh, Float farDistance) {
	auto closestDistance2 = std::numeric_limits<Float>::infinity();
	auto closestPoint = glm::vec3(0.0f);
	auto closestTriangle = Triangle();

	for (auto f = SizeType(0); f < mesh.getNumberOfFaces(); ++f) {
		const auto triangle = mesh.getFaceTriangle(f);
		const auto pointOnTriangle = triangle.getClosestPoint(point);
		const auto distance2 = Float(glm::distance2(pointOnTriangle, point));

		if (distance2 < closestDistance2) {
			closestTriangle = triangle;
			closestPoint = pointOnTriangle;
			closestDistance2 = distance2;
		}
	}
	
	assert(!std::isinf(closestDistance2));

	auto closestDistance = Float(std::sqrt(closestDistance2));

	if (_isPointInsideMesh(point, mesh, farDistance)) {
		closestDistance *= -1.0f; // inside distances are negative
	}

	return closestDistance;
}

}

std::unique_ptr<TriangleMeshDistanceField> TriangleMeshDistanceFieldBuilder::_create(std::unique_ptr<const TriangleMesh> mesh, const glm::vec3& cellsSize, const glm::vec3& aabbVolumeExpansion) const {
	auto meshDistanceField = std::make_unique<TriangleMeshDistanceField>();
	meshDistanceField->_mesh = std::move(mesh);

	auto aabb = meshDistanceField->_mesh->calculateAABB();
	aabb.expand(aabbVolumeExpansion);
	const auto aabbSize = aabb.getExtensions();

	const auto widthInCells = SizeType(std::ceil(aabbSize.x / cellsSize.x));
	const auto heightInCells = SizeType(std::ceil(aabbSize.y / cellsSize.y));
	const auto depthInCells = SizeType(std::ceil(aabbSize.z / cellsSize.z));

	meshDistanceField->_voxelGrid = FloatVoxelGrid(
		widthInCells + SizeType(1),
		heightInCells + SizeType(1),
		depthInCells + SizeType(1),
		aabb
	);

	return std::move(meshDistanceField);
}

void TriangleMeshDistanceFieldBuilder::_calculateDistances(TriangleMeshDistanceField& meshDistanceField) const {
	const auto farDistance = meshDistanceField._voxelGrid.getAABB().getVolume();

	auto count = SizeType(0);
	auto lastProgress = Float(0.0f);

	const auto updateProgressBar = [&count, &lastProgress, &meshDistanceField]() {
		const auto progress = Float(100.0f) * (Float(++count) / Float(meshDistanceField._voxelGrid.getNumberOfVoxels()));

		if ((progress - lastProgress) >= 1.0f) {
			printf("[TriangleMeshDistanceField] Calculating distances ... %.0f%% - %d/%d\n", progress, count, meshDistanceField._voxelGrid.getNumberOfVoxels()); // progress bar
			lastProgress = progress;
		}
	};

	for (auto xIdx = SizeType(0); xIdx < meshDistanceField._voxelGrid.getWidth(); ++xIdx) {
		for (auto yIdx = SizeType(0); yIdx < meshDistanceField._voxelGrid.getHeight(); ++yIdx) {
			for (auto zIdx = SizeType(0); zIdx < meshDistanceField._voxelGrid.getDepth(); ++zIdx) {
				const auto voxelIndex = Index3D(xIdx, yIdx, zIdx);
				const auto voxelPosition = meshDistanceField._voxelGrid.getVoxelPosition(voxelIndex);

				meshDistanceField._voxelGrid.setVoxel(
					voxelIndex,
					_calculateDistance(voxelPosition, *meshDistanceField._mesh, farDistance)
				);

				updateProgressBar();
			}
		}
	}
}

std::unique_ptr<TriangleMeshDistanceField> TriangleMeshDistanceFieldBuilder::build(std::unique_ptr<const TriangleMesh> mesh, const glm::vec3& cellsSize, const glm::vec3& aabbVolumeExpansion) const {
	auto meshDistanceField = _create(std::move(mesh), cellsSize, aabbVolumeExpansion);

	if (meshDistanceField) {
		_calculateDistances(*meshDistanceField);
	}

	return std::move(meshDistanceField);
}

}
