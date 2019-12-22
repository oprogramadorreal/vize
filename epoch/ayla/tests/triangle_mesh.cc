#include <boost/test/unit_test.hpp>

#include <ayla/geometry/triangle_mesh.hpp>
#include <ayla/geometry/vector.hpp>

BOOST_AUTO_TEST_SUITE(ayla)
BOOST_AUTO_TEST_SUITE(triangle_mesh)

const Float _positions[] = { 
	 4.0f, -4.0f, // 0
	-4.0f, -4.0f, // 1
	 4.0f,  4.0f, // 2
	-4.0f,  4.0f, // 3
	 0.0f, -4.0f, // 4
	 0.0f,  4.0f, // 5
	-4.0f,  0.0f, // 6 
	 4.0f,  0.0f, // 7
	 0.0f,  0.0f  // 8
};
	
const TriangularFace::IndexType _f[] = {
	2, 5, 8, // 0
	0, 7, 4, // 1
	8, 6, 1, // 2
	8, 5, 6, // 3
	5, 3, 6, // 4
	4, 8, 1, // 5
	4, 7, 8, // 6
	7, 2, 8  // 7
};

/*
 * Mesh configuration:
 *
 * 3__5__2
 * | /| /|
 * 6/_8/_7
 * | /| /|
 * 1/_4/_0
 */

const std::set<SizeType> _faceNeighbors[] = {
	{ 3u, 7u }, // 0
	{ 6u }, // 1
	{ 3u, 5u }, // 2
	{ 0u, 2u, 4u }, // 3
	{ 3u }, // 4
	{ 2u, 6u }, // 5
	{ 1u, 5u, 7u }, // 6
	{ 0u, 6u } // 7u
};

const std::array<UnorderedPair<SizeType>, 3u> _faceEdges[] = {
	{ UnorderedPair<SizeType>(2, 5), UnorderedPair<SizeType>(5, 8), UnorderedPair<SizeType>(2, 8) }, // 0
	{ UnorderedPair<SizeType>(0, 7), UnorderedPair<SizeType>(7, 4), UnorderedPair<SizeType>(4, 0) }, // 1
	{ UnorderedPair<SizeType>(8, 6), UnorderedPair<SizeType>(6, 1), UnorderedPair<SizeType>(1, 8) }, // 2
	{ UnorderedPair<SizeType>(8, 5), UnorderedPair<SizeType>(5, 6), UnorderedPair<SizeType>(6, 8) }, // 3
	{ UnorderedPair<SizeType>(5, 3), UnorderedPair<SizeType>(3, 6), UnorderedPair<SizeType>(6, 5) }, // 4
	{ UnorderedPair<SizeType>(4, 8), UnorderedPair<SizeType>(8, 1), UnorderedPair<SizeType>(1, 4) }, // 5
	{ UnorderedPair<SizeType>(4, 7), UnorderedPair<SizeType>(7, 8), UnorderedPair<SizeType>(8, 4) }, // 6
	{ UnorderedPair<SizeType>(7, 2), UnorderedPair<SizeType>(2, 8), UnorderedPair<SizeType>(8, 7) }  // 7u
};

const std::array<UnorderedPair<SizeType>, 16u> _meshEdges = {
	UnorderedPair<SizeType>(3, 5), // horizontal
	UnorderedPair<SizeType>(5, 2),
	UnorderedPair<SizeType>(6, 8),
	UnorderedPair<SizeType>(8, 7),
	UnorderedPair<SizeType>(1, 4),
	UnorderedPair<SizeType>(4, 0),
	UnorderedPair<SizeType>(3, 6), // vertical
	UnorderedPair<SizeType>(6, 1),
	UnorderedPair<SizeType>(5, 8),
	UnorderedPair<SizeType>(8, 4),
	UnorderedPair<SizeType>(2, 7),
	UnorderedPair<SizeType>(7, 0),
	UnorderedPair<SizeType>(6, 5), // diagonal
	UnorderedPair<SizeType>(8, 2),
	UnorderedPair<SizeType>(1, 8),
	UnorderedPair<SizeType>(4, 7)
};

TriangleMesh buildTestTriangleMesh( ) {
	std::vector<Vertex> _vertices;
	std::vector<TriangularFace> _faces;
	
	for (int i = 0; i < 9; i++) {
		Vertex v;
		v.setPosition(glm::vec3(_positions[ 2*i ], _positions[ 2*i + 1 ], 0.0f));
		v.setNormal(glm::vec3(0.0f, 0.0f, 1.0f));
		_vertices.push_back(v);
	}
	
	for (int i = 0; i < 8; i++) {
		TriangularFace f( _f[ 3*i ], _f[ 3*i + 1 ], _f[ 3*i + 2 ] );
		_faces.push_back( f );
	}
	
	return TriangleMesh( _vertices, _faces );
}

BOOST_AUTO_TEST_CASE(getters_and_setters) {
	TriangleMesh mesh = buildTestTriangleMesh();
	BOOST_REQUIRE(mesh.checkIntegrity());

	BOOST_CHECK(ayla::epsilonEqual(mesh.getVertex(0).getPosition(), glm::vec3(4.0f, -4.0f, 0.0f)));

	Vertex before = mesh.getVertex(0);
	Vertex other;
	other.setPosition(glm::vec3(10.0f, 5.0f, glm::pi<Float>()));

	mesh.setVertex(0, other);
	BOOST_CHECK(ayla::epsilonEqual(mesh.getVertex(0).getPosition(), other.getPosition()));
	mesh.setVertex(0, before);
	BOOST_CHECK(ayla::epsilonEqual(mesh.getVertex(0).getPosition(), before.getPosition()));
}

BOOST_AUTO_TEST_CASE(connectivity) {
	TriangleMesh mesh = buildTestTriangleMesh();
	BOOST_REQUIRE(mesh.checkIntegrity());

	BOOST_CHECK(mesh.getNumberOfVertices() == 9);
	BOOST_CHECK(mesh.getNumberOfFaces() == 8);

	auto neighbors = mesh.getVerticesConnectedToVertex(0);
	BOOST_CHECK(neighbors.size() == 2);
	BOOST_CHECK(neighbors.find(4) != neighbors.end());
	BOOST_CHECK(neighbors.find(7) != neighbors.end());

	neighbors = mesh.getVerticesConnectedToVertex(5);
	BOOST_CHECK(neighbors.size() == 4);
	BOOST_CHECK(neighbors.find(2) != neighbors.end());
	BOOST_CHECK(neighbors.find(3) != neighbors.end());
	BOOST_CHECK(neighbors.find(6) != neighbors.end());
	BOOST_CHECK(neighbors.find(8) != neighbors.end());

	neighbors = mesh.getVerticesConnectedToVertex(8);
	BOOST_CHECK(neighbors.size() == 6);
	BOOST_CHECK(neighbors.find(1) != neighbors.end());
	BOOST_CHECK(neighbors.find(2) != neighbors.end());
	BOOST_CHECK(neighbors.find(4) != neighbors.end());
	BOOST_CHECK(neighbors.find(5) != neighbors.end());
	BOOST_CHECK(neighbors.find(6) != neighbors.end());
	BOOST_CHECK(neighbors.find(7) != neighbors.end());

	auto neighborFaces = mesh.getFacesConnectedToVertex(0);
	BOOST_CHECK(neighborFaces.size() == 1);
	BOOST_CHECK(neighborFaces.find(1) != neighborFaces.end());

	neighborFaces = mesh.getFacesConnectedToVertex(5);
	BOOST_CHECK(neighborFaces.size() == 3);
	BOOST_CHECK(neighborFaces.find(0) != neighborFaces.end());
	BOOST_CHECK(neighborFaces.find(3) != neighborFaces.end());
	BOOST_CHECK(neighborFaces.find(4) != neighborFaces.end());

	neighborFaces = mesh.getFacesConnectedToVertex(8);
	BOOST_CHECK(neighborFaces.size() == 6);
	BOOST_CHECK(neighborFaces.find(0) != neighborFaces.end());
	BOOST_CHECK(neighborFaces.find(2) != neighborFaces.end());
	BOOST_CHECK(neighborFaces.find(3) != neighborFaces.end());
	BOOST_CHECK(neighborFaces.find(5) != neighborFaces.end());
	BOOST_CHECK(neighborFaces.find(6) != neighborFaces.end());
	BOOST_CHECK(neighborFaces.find(7) != neighborFaces.end());

	BOOST_CHECK(mesh.findFace(2, 5, 8) == 0);
	BOOST_CHECK(mesh.findFace(4, 7, 8) == 6);
	BOOST_CHECK(mesh.findFace(1, 2, 3) == std::numeric_limits<SizeType>::max());
	BOOST_CHECK(mesh.findFace(0, 8, 10) == std::numeric_limits<SizeType>::max());
}

BOOST_AUTO_TEST_CASE(aabb) {
	TriangleMesh mesh = buildTestTriangleMesh();
	BOOST_REQUIRE(mesh.checkIntegrity());

	glm::vec3 minPoint, maxPoint;
	mesh.calculateAABB(minPoint, maxPoint);
	AxisAlignedBox aab = mesh.calculateAABB();

	BOOST_CHECK(ayla::epsilonEqual(minPoint, glm::vec3(-4.0f, -4.0f, 0.0f)));
	BOOST_CHECK(ayla::epsilonEqual(maxPoint, glm::vec3(4.0f, 4.0f, 0.0f)));
	BOOST_CHECK(ayla::epsilonEqual(minPoint, aab.getMinPoint()));
	BOOST_CHECK(ayla::epsilonEqual(maxPoint, aab.getMaxPoint()));
}

BOOST_AUTO_TEST_CASE(planarity) {
	TriangleMesh mesh = buildTestTriangleMesh();
	BOOST_REQUIRE(mesh.checkIntegrity());

	BOOST_CHECK(mesh.isPlanar());

	Vertex before = mesh.getVertex(3);
	Vertex highV;
	highV.setPosition(glm::vec3(before.getPosition().x, before.getPosition().y, 10.0f));
	highV.setNormal(before.getNormal());
	mesh.setVertex(3, highV);

	BOOST_CHECK(!mesh.isPlanar());
	mesh.setVertex(3, before);
	BOOST_CHECK(mesh.isPlanar());
}

BOOST_AUTO_TEST_CASE(face_triangles) {
	TriangleMesh mesh = buildTestTriangleMesh();
	BOOST_REQUIRE(mesh.checkIntegrity());

	Triangle faceA = mesh.getFaceTriangle(0);
	Triangle faceB = mesh.getFaceTriangle(5);

	BOOST_CHECK(ayla::epsilonEqual(faceA.getA(), glm::vec3(4.0f, 4.0f, 0.0f)));
	BOOST_CHECK(ayla::epsilonEqual(faceA.getB(), glm::vec3(0.0f, 4.0f, 0.0f)));
	BOOST_CHECK(ayla::epsilonEqual(faceA.getC(), glm::vec3(0.0f, 0.0f, 0.0f)));

	BOOST_CHECK(ayla::epsilonEqual(faceB.getA(), glm::vec3(0.0f, -4.0f, 0.0f)));
	BOOST_CHECK(ayla::epsilonEqual(faceB.getB(), glm::vec3(0.0f, 0.0f, 0.0f)));
	BOOST_CHECK(ayla::epsilonEqual(faceB.getC(), glm::vec3(-4.0f, -4.0f, 0.0f)));
}

BOOST_AUTO_TEST_CASE(closest_vertex) {
	TriangleMesh mesh = buildTestTriangleMesh();
	BOOST_REQUIRE(mesh.checkIntegrity());

	std::pair< SizeType, Float > a = mesh.getClosestVertex(glm::vec3(-5.0f, 0.0f, 0.0f));

	BOOST_CHECK(ayla::epsilonEqual(mesh.getVertex(a.first).getPosition(), glm::vec3(-4.0f, 0.0f, 0.0f)));
	BOOST_CHECK(a.first == 6);

	std::pair< SizeType, Float > b = mesh.getClosestVertex(glm::vec3(0.0f, 0.0f, 0.4f));

	BOOST_CHECK(ayla::epsilonEqual(mesh.getVertex(b.first).getPosition(), glm::vec3(0.0f, 0.0f, 0.0f)));
	BOOST_CHECK(b.first == 8);

	BOOST_CHECK(mesh.getExtremeVertexIndex(glm::normalize(glm::vec3(1.0f, -1.0f, 0.0f))) == 0);
	BOOST_CHECK(mesh.getExtremeVertexIndex(glm::normalize(glm::vec3(-1.0f, -1.0f, 0.0f))) == 1);
	BOOST_CHECK(mesh.getExtremeVertexIndex(glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f))) == 2);
	BOOST_CHECK(mesh.getExtremeVertexIndex(glm::normalize(glm::vec3(-1.0f, 1.0f, 0.0f))) == 3);
}

BOOST_AUTO_TEST_CASE(invert_normals) {
	TriangleMesh mesh = buildTestTriangleMesh();
	BOOST_REQUIRE(mesh.checkIntegrity());

	mesh.invertFacesOrientation();

	for (SizeType i = 0; i < mesh.getNumberOfFaces(); i++) {
		auto face = mesh.getFace(i);

		BOOST_CHECK(face.getA() == _f[3 * i]);
		BOOST_CHECK(face.getB() == _f[3 * i + 2]);
		BOOST_CHECK(face.getC() == _f[3 * i + 1]);
	}
}

BOOST_AUTO_TEST_CASE(area) {
	TriangleMesh mesh = buildTestTriangleMesh();
	BOOST_REQUIRE(mesh.checkIntegrity());

	BOOST_CHECK(mesh.calculateArea() == 64);
}

TriangleMesh divideAllFaces(const TriangleMesh& mesh) {
	TriangleMesh result = mesh;

	const SizeType originalNumberOfFaces = mesh.getNumberOfFaces();

	for (SizeType i = 0u; i < originalNumberOfFaces; ++i) {
		result.divideFaceInThreeByCentroid(i);
	}

	return result;
}

BOOST_AUTO_TEST_CASE(divide_face) {
	TriangleMesh originalMesh = buildTestTriangleMesh();
	BOOST_REQUIRE(originalMesh.checkIntegrity());

	const SizeType numberOfDivisions = 3u;

	for (SizeType i = 0u; i < numberOfDivisions; ++i) {
		const TriangleMesh dividedMesh = divideAllFaces(originalMesh);
		
		BOOST_CHECK(dividedMesh.checkIntegrity());
		BOOST_CHECK(dividedMesh.getNumberOfFaces() == (3 * originalMesh.getNumberOfFaces()));
		BOOST_CHECK(dividedMesh.getNumberOfVertices() == (originalMesh.getNumberOfVertices() + originalMesh.getNumberOfFaces()));

		originalMesh = dividedMesh;
	}
}

namespace {

bool _compare(const std::set<SizeType>& a, const std::set<SizeType>& b) {
	std::set<SizeType> difference;
	std::set_difference(a.begin(), a.end(), b.begin(), b.end(), std::inserter(difference, difference.begin()));
	return difference.size() == 0;
}

bool _compare(const std::array<UnorderedPair<SizeType>, 3u>& a, const std::array<UnorderedPair<SizeType>, 3u>& b) {
	for (SizeType i = 0u; i < 3u; ++i) {
		if (a[i] != b[i]) {
			return false;
		}
	}

	return true;
}

}

BOOST_AUTO_TEST_CASE(face_neighbors) {
	const TriangleMesh mesh = buildTestTriangleMesh();

	for (SizeType i = 0u; i < mesh.getNumberOfFaces(); ++i) {
		BOOST_CHECK(_compare(mesh.getFaceNeighbors(i), _faceNeighbors[i]));
	}
}

BOOST_AUTO_TEST_CASE(face_edges) {
	const TriangleMesh mesh = buildTestTriangleMesh();

	for (SizeType i = 0u; i < mesh.getNumberOfFaces(); ++i) {
		BOOST_CHECK(_compare(mesh.getFaceEdges(i), _faceEdges[i]));
	}
}

BOOST_AUTO_TEST_CASE(edges) {
	const TriangleMesh mesh = buildTestTriangleMesh();
	const auto calculatedMeshEdges = mesh.getEdges();

	for (auto it = _meshEdges.begin(); it != _meshEdges.end(); ++it) {
		BOOST_CHECK(calculatedMeshEdges.find(*it) != calculatedMeshEdges.end());
	}
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
