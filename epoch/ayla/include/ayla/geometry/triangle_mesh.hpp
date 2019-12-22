#ifndef AYLA_TRIANGLE_MESH_HPP
#define AYLA_TRIANGLE_MESH_HPP

#include "ayla/config.hpp"

#include "ayla/geometry/triangle.hpp"
#include "ayla/geometry/ray.hpp"
#include "ayla/geometry/vertex.hpp"
#include "ayla/geometry/triangular_face.hpp"

#include "ayla/pair.hpp"

#include "ayla/serialization/triangle_mesh_serializer.hpp"

#include <vector>

namespace ayla {

/**
 * A mesh of triangles.
 *
 * This is supposed to be simple. Internal representation is expected to be just
 * an array of vertices and an array of faces telling you how to connect these vertices.
 * This simplicity makes it straightforward to send mesh data to OpenGL, for example.
 * However, since we do not explicitly store any edges or neighboring information, some queries may be slow.
 *
 * @see ayla::TriangleMeshBuilder
 * @see ayla::Vertex
 * @see ayla::TriangularFace
 *
 * @author O Programador
 */
class AYLA_API TriangleMesh final {
public:
	TriangleMesh();

	TriangleMesh(const std::vector<Vertex>& vertices, const std::vector<TriangularFace>& faces);

	/**
	 * Creates a mesh to accommodate @param numberOfVertices vertices and @param numberOfFaces faces.
	 *
	 * All vertices and faces values will be invalid (uninitialized).
	 * User must explicitly set vertices and faces values after this (using "setVertex" and "setFace" methods).
	 */
	TriangleMesh(SizeType numberOfVertices, SizeType numberOfFaces);
	
	SizeType getNumberOfVertices() const;
	SizeType getNumberOfFaces() const;

	const Vertex& getVertex(SizeType i) const;
	Vertex& getVertex(SizeType i);

	const TriangularFace& getFace(SizeType i) const;
	TriangularFace& getFace(SizeType i);

	void setVertex(SizeType i, const Vertex& v);
	void setFace(SizeType i, const TriangularFace& f);

	Triangle getFaceTriangle(SizeType faceIndex) const;
	Triangle getFaceTriangle(const TriangularFace& face) const;

	void calculateNormals();

	/**
	 * Calculates the min and max points of an axis-aligned bounding box (AABB) for this triangle mesh.
	 */
	void calculateAABB(glm::vec3& minPoint, glm::vec3& maxPoint) const;

	AxisAlignedBox calculateAABB() const;

	void modifyFacesOrientation(const glm::vec3& normal);

	/**
	 * Invert the order of the vertices in each face.
	 * Transforms the face (va, vb, vc) into (va, vc, vb). 
	 * This means that calculateNormals will calculate normals to the opposite side.
	 */
	void invertFacesOrientation();

	/**
	 * Add all the vertices and faces of other triangle mesh to this one.
	 */
	void append(const TriangleMesh& other);

	bool isPlanar(Float threshold = 0.000001f) const;

	Float calculateArea() const;

	/**
	 * @return A pair containing the index of the vertex of this mesh that is the closest to @param point and the distance to that point.
	 */
	std::pair<SizeType, Float> getClosestVertex(const glm::vec3& point) const;

	/**
	 * @return The index of the most extreme vertex in the desired direction.
	 */
	SizeType getExtremeVertexIndex(const glm::vec3& direction) const;

	/**
	 * Returns the indices of the faces that share a specified vertex.
	 */
	std::set<SizeType> getFacesConnectedToVertex(SizeType vertexIndex) const;

	/**
	 * Returns the indices of the vertices that are adjacent of vertex @param vertexIndex.
	 */
	std::set<SizeType> getVerticesConnectedToVertex(SizeType vertexIndex) const;

	/**
	 * @return A set containing the indices of faces adjacent to the face specified by @param faceIndex.
	 */
	std::set<SizeType> getFaceNeighbors(SizeType faceIndex) const;

	/**
	 * @return An array containing three pairs of indices which represent the edges of the face specified by @param faceIndex.
	 */
	std::array<UnorderedPair<SizeType>, 3u> getFaceEdges(SizeType faceIndex) const;

	/**
	 * @return A set containing the pairs of vertices conected by triangles (the edges) in this triangle mesh.
	 */
	std::set<UnorderedPair<SizeType> > getEdges() const;

	/**
	 * @return TriangularFace index on success or std::numeric_limits<SizeType>::max() if face (va, vb, vc) was not found.
	 */
	SizeType findFace(SizeType va, SizeType vb, SizeType vc) const;

	/**
	 * @return TRUE if there is no unused vertex and no invalid face.
	 *
	 * That is:
	 * (1) Each vertex is referenced by at least one face.
	 * (2) Each face points to 3 valid vertices of this mesh.
	 *
	 * Note that (1) and (2) are both valid when the mesh is empty (has no vertices and no faces).
	 */
	bool checkIntegrity() const;

	/**
	 * Divide one face in three by adding a new vertex on the face center (face triangle centroid).
	 *
	 * Note that this will add one new vertex but only two new faces:
	 * the divided face will be kept and it will start pointing to the new vertex.
	 *
	 * The 3 new features (1 vertex and 2 faces) will be added at the end of the
	 * containers (vertices and faces vectors), thus you can easily get their indices.
	 */
	void divideFaceInThreeByCentroid(SizeType faceIndex);

	/**
	 * Rotate all the vertices of the triangle mesh around @param rotationCenter by @param rotation.
	 */
	void rotate(const glm::quat& rotation, const glm::vec3& rotationCenter);

	/**
	 * Rotate all the vertices of the triangle mesh around its center by @param rotation.
	 */
	void rotate(const glm::quat& rotation);

	/**
	 * Move @param basePosition to @param newPosition and apply the same displacement
	 * into all vertices of the triangle mesh.
	 */
	void setPosition(const glm::vec3& newPosition, const glm::vec3& basePosition);

	/**
	 * Move the center of the triangle mesh to @param newPosition
	 * and apply the same displacement into all vertices of the triangle mesh.
	 */
	void setPosition(const glm::vec3& newPosition);

	/**
	 * @return A copy of this mesh transformed by @param matrix.
	 */
	TriangleMesh transformed(const glm::mat4& matrix) const;

	/**
	 * Transform this mesh by @param matrix.
	 */
	void transform(const glm::mat4& matrix);

	/**
	 * @return The center of the whole triangle mesh (including all sub meshes).
	 */
	glm::vec3 getCenter() const;

	/**
	 * Calculate the average edge length and its standard deviation.
	 *
	 * @return A std::pair containing the average (first) and the standard deviation (second).
	 */
	std::pair<Float, Float> getEdgeLengthStatistics() const;

	std::pair<Float, Float> getEdgeLengthStatistics(const std::set<UnorderedPair<SizeType> >& edges) const;

	/**
	 * Remove vertices that are not referenced by any face.
	 * Faces will be modified.
	 */
	void removeLooseVertices();

public:
	struct RayQueryData final {
		SizeType faceIndex = std::numeric_limits<SizeType>::max();
		Float distance2 = Float(0.0f); // squared distance to ray origin

		inline bool operator<(const RayQueryData& other) const { return distance2 < other.distance2; }
	};

	using RayQueryResult = std::vector<RayQueryData>;
	
	/**
	 * @remarks The returned float values are distances to ray origin.
	 */
	RayQueryResult rayQuery(const Ray& ray, bool sortResult = false) const;

	/**
	 * @remarks The returned float values are distances to segment origin (P).
	 */
	RayQueryResult rayQuery(const LineSegment& segment, bool sortResult = false) const;

public:
	/**
	 * @return A vector containing all the vertices in this mesh.
	 */
	inline const std::vector<Vertex>& getVertices() const { return _vertices; }

	/**
	 * @return A vector containing all the faces in this mesh.
	 */
	inline const std::vector<TriangularFace>& getFaces() const { return _faces; }

private:
	std::vector<Vertex> _vertices; // vertices data
	std::vector<TriangularFace> _faces; // faces data

	template<class Archive> friend void boost::serialization::serialize(Archive&, ayla::TriangleMesh&, const unsigned int);
};

}

#endif // AYLA_TRIANGLE_MESH_HPP