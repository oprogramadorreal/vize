#include "ayla/geometry/triangle_mesh_builder.hpp"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace ayla {

std::unique_ptr<TriangleMesh> TriangleMeshBuilder::buildUVSphere(SizeType rings, SizeType sectors, Float radius) {
	assert(rings > 0u);
	assert(sectors > 0u);
	assert(radius > 0.0f);

	const Float pi = glm::pi<Float>();
	const Float piOver2 = glm::pi<Float>() / 2.0f;

	const Float R = 1.0f / Float(rings - 1);
	const Float S = 1.0f / Float(sectors - 1);

	std::vector<Vertex> vertices(rings * sectors);
	auto vIt = vertices.begin();

	for (int r = 0; r < int(rings); ++r) {
		for (int s = 0; s < int(sectors); ++s) {
			const Float y = std::sin(Float(-piOver2 + pi * r * R));
			const Float x = std::cos(Float(2 * pi * s * S)) * std::sin(Float(pi * r * R));
			const Float z = std::sin(Float(2 * pi * s * S)) * std::sin(Float(pi * r * R));

			glm::vec2 uv;
			uv.x = s*S;
			uv.y = r*R;
			vIt->setUV(uv);

			glm::vec3 position;
			position.x = x * radius;
			position.y = y * radius;
			position.z = z * radius;
			vIt->setPosition(position);

			glm::vec3 normal;
			normal.x = x;
			normal.y = y;
			normal.z = z;
			vIt->setNormal(normal);

			++vIt;
		}
	}

	std::vector<TriangularFace> faces( (rings - 1) * (sectors - 1) * 2);
	auto fIt = faces.begin();

	for (int r = 0; r < int(rings - 1); ++r) {
		for (int s = 0; s < int(sectors - 1); ++s) {
			fIt->setA(TriangularFace::IndexType(r * sectors + s)); // 0 0
			fIt->setB(TriangularFace::IndexType((r + 1) * sectors + (s + 1))); // 1 1
			fIt->setC(TriangularFace::IndexType(r * sectors + (s + 1))); // 0 1
			++fIt;

			fIt->setA(TriangularFace::IndexType((r + 1) * sectors + (s + 1))); // 1 1
			fIt->setB(TriangularFace::IndexType(r * sectors + s)); // 0 0
			fIt->setC(TriangularFace::IndexType((r + 1) * sectors + s)); // 1 0
			++fIt;
		}
	}

	return std::make_unique<TriangleMesh>(vertices, faces);
}

std::unique_ptr<TriangleMesh> TriangleMeshBuilder::buildCube(Float sideLength) {
	const Float halfSideLength = sideLength / 2.0f;

	const auto createCubeVertex = [halfSideLength](short i, short j, short k) {
		const glm::vec3 position = glm::vec3(i*halfSideLength, j*halfSideLength, k*halfSideLength);
		return Vertex(position, position, glm::vec2(0, 0)); // TODO: texture mapping
	};

	std::vector<Vertex> vertices(8);
	vertices[0] = createCubeVertex(-1, -1, -1);
	vertices[1] = createCubeVertex(-1, -1,  1);
	vertices[2] = createCubeVertex(-1,  1, -1);
	vertices[3] = createCubeVertex(-1,  1,  1);
	vertices[4] = createCubeVertex( 1, -1, -1);
	vertices[5] = createCubeVertex( 1, -1,  1);
	vertices[6] = createCubeVertex( 1,  1, -1);
	vertices[7] = createCubeVertex( 1,  1,  1);

	// normalize normal vectors
	for (SizeType i = 0u; i < vertices.size(); ++i) {
		vertices[i].setNormal(glm::normalize(vertices[i].getNormal()));
	}

	std::vector<TriangularFace> faces(12);
	faces[0] = TriangularFace(3, 1, 7); // front
	faces[1] = TriangularFace(1, 5, 7);
	faces[2] = TriangularFace(2, 0, 3); // left
	faces[3] = TriangularFace(0, 1, 3);
	faces[4] = TriangularFace(6, 0, 2); // back
	faces[5] = TriangularFace(6, 4, 0);
	faces[6] = TriangularFace(7, 5, 6); // right
	faces[7] = TriangularFace(5, 4, 6);
	faces[8] = TriangularFace(2, 3, 7); // top
	faces[9] = TriangularFace(2, 7, 6);
	faces[10] = TriangularFace(1, 0, 5); // bottom
	faces[11] = TriangularFace(0, 4, 5);

	return std::make_unique<TriangleMesh>(vertices, faces);
}

}