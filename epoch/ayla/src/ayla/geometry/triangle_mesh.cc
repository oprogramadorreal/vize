#include "ayla/geometry/triangle_mesh.hpp"
#include "ayla/geometry/intersection.hpp"

#include <glm/gtc/epsilon.hpp>

#include <algorithm>

namespace ayla {

TriangleMesh::TriangleMesh() = default;

TriangleMesh::TriangleMesh(const std::vector<Vertex>& vertices, const std::vector<TriangularFace>& faces)
	: _vertices(vertices), _faces(faces)
{ }

TriangleMesh::TriangleMesh(SizeType numberOfVertices, SizeType numberOfFaces)
	: _vertices(numberOfVertices), _faces(numberOfFaces)
{ }

SizeType TriangleMesh::getNumberOfVertices() const {
	return SizeType(_vertices.size());
}

SizeType TriangleMesh::getNumberOfFaces() const {
	return SizeType(_faces.size());
}

const Vertex& TriangleMesh::getVertex(SizeType i) const {
	assert(i < getNumberOfVertices());
	return _vertices[i];
}

Vertex& TriangleMesh::getVertex(SizeType i) {
	assert(i < getNumberOfVertices());
	return _vertices[i];
}

const TriangularFace& TriangleMesh::getFace(SizeType i) const {
	assert(i < getNumberOfFaces());
	return _faces[i];
}

TriangularFace& TriangleMesh::getFace(SizeType i) {
	assert(i < getNumberOfFaces());
	return _faces[i];
}

void TriangleMesh::setVertex(SizeType i, const Vertex& v) {
	assert(i < getNumberOfVertices());
	_vertices[i] = v;
}

Triangle TriangleMesh::getFaceTriangle(SizeType faceIndex) const {
	return getFaceTriangle(getFace(faceIndex));
}
Triangle TriangleMesh::getFaceTriangle(const TriangularFace& face) const {
	return Triangle(getVertex(face.getA()).getPosition(), getVertex(face.getB()).getPosition(), getVertex(face.getC()).getPosition());
}

void TriangleMesh::setFace(SizeType i, const TriangularFace& f) {
	assert(i < getNumberOfFaces());
	_faces[i] = f;
}

void TriangleMesh::calculateNormals() {
	// zero all normals - they will be recalculated below
	for (auto i = SizeType(0); i < getNumberOfVertices(); ++i) {
		_vertices[i].setNormal(glm::vec3(0.0f, 0.0f, 0.0f));
	}

	// calculate normal directions
	for (auto i = SizeType(0); i < getNumberOfFaces(); ++i) {
		const auto& fi = _faces[i];

		auto& va = _vertices[fi.getA()];
		auto& vb = _vertices[fi.getB()];
		auto& vc = _vertices[fi.getC()];

		const auto normal = glm::cross(vb.getPosition() - va.getPosition(), vc.getPosition() - va.getPosition()); // calculate face i non-normalized normal vector
		va.setNormal(va.getNormal() + normal); // accumulate face i normal for vertex a
		vb.setNormal(vb.getNormal() + normal); // accumulate face i normal for vertex b
		vc.setNormal(vc.getNormal() + normal); // accumulate face i normal for vertex c
	}

	// normalize all normal vectors
	for (auto i = SizeType(0); i < getNumberOfVertices(); ++i) {
		_vertices[i].setNormal(glm::normalize(_vertices[i].getNormal()));
	}
}

void TriangleMesh::calculateAABB(glm::vec3& minPoint, glm::vec3& maxPoint) const {
	minPoint = glm::vec3(std::numeric_limits<Float>::max());
	maxPoint = glm::vec3(-std::numeric_limits<Float>::max());

	for (auto i = SizeType(0); i < SizeType(getNumberOfVertices()); ++i) {
		const glm::vec3& vPosition = getVertex(i).getPosition();

		for (auto j = SizeType(0); j < SizeType(3); ++j) {
			if (vPosition[j] < minPoint[j]) {
				minPoint[j] = vPosition[j];
			}

			if (vPosition[j] > maxPoint[j]) {
				maxPoint[j] = vPosition[j];
			}
		}
	}
}

AxisAlignedBox TriangleMesh::calculateAABB() const {
	glm::vec3 minPoint, maxPoint;
	calculateAABB(minPoint, maxPoint);
	return AxisAlignedBox(minPoint, maxPoint);
}

void TriangleMesh::modifyFacesOrientation(const glm::vec3& incidentVector) {
	for (auto it = _faces.begin(); it != _faces.end(); ++it) {
		if (glm::dot(getFaceTriangle(*it).getNormal(), incidentVector) < 0.0f) {
			it->flip();
		}
	}
}

void TriangleMesh::invertFacesOrientation() {
	for (auto it = _faces.begin(); it != _faces.end(); ++it) {
		it->flip();
	}
}

void TriangleMesh::append(const TriangleMesh& other) {
	const auto vSizeBefore = SizeType(_vertices.size());

	_vertices.insert(_vertices.end(), other._vertices.begin(), other._vertices.end());
	_faces.reserve(_faces.size() + other._faces.size());

	for (auto otherFace : other._faces) {
		_faces.emplace_back(
			TriangularFace::IndexType(otherFace.getA() + vSizeBefore),
			TriangularFace::IndexType(otherFace.getB() + vSizeBefore),
			TriangularFace::IndexType(otherFace.getC() + vSizeBefore)
		);
	}
}

bool TriangleMesh::isPlanar(Float threshold) const {
	if (getNumberOfFaces() == 0) {
		return false;
	}

	if (getNumberOfFaces() == 1u) {
		return true;
	}

	const glm::vec3 f0Normal = getFaceTriangle(0).getNormal();

	for (SizeType i = 1u; i < getNumberOfFaces(); ++i) {
		if (glm::epsilonNotEqual(glm::dot(getFaceTriangle(i).getNormal(), f0Normal), 1.0f, threshold)) {
			return false;
		}
	}

	return true;
}

Float TriangleMesh::calculateArea() const {
	Float area = 0.0f;

	for (SizeType i = 0u; i < getNumberOfFaces(); ++i) {
		area += getFaceTriangle(i).calculateArea();
	}

	return area;
}

std::pair<SizeType, Float> TriangleMesh::getClosestVertex(const glm::vec3& point) const {
	Float smallestDistance2 = std::numeric_limits<Float>::infinity();
	SizeType bestIndex = std::numeric_limits<SizeType>::max();

	for (SizeType i = 0u; i < getNumberOfVertices(); ++i) {
		const Float distance2 = glm::distance2(_vertices[i].getPosition(), point);

		if (distance2 < smallestDistance2) {
			smallestDistance2 = distance2;
			bestIndex = i;
		}
	}

	return std::make_pair(bestIndex, std::sqrt(smallestDistance2));
}

SizeType TriangleMesh::getExtremeVertexIndex(const glm::vec3& direction) const {
	assert(glm::epsilonEqual(1.0f, glm::length(direction), 0.0000001f));

	Float fartest = -std::numeric_limits<Float>::max();
	SizeType fartestIndex = std::numeric_limits<SizeType>::max();

	for (SizeType i = 0u; i < getNumberOfVertices(); ++i) {
		const Float projection = glm::dot(getVertex(i).getPosition(), direction);

		if (projection > fartest) {
			fartest = projection;
			fartestIndex = i;
		}
	}

	assert(fartestIndex != std::numeric_limits<SizeType>::max());
	return fartestIndex;
}

std::set<SizeType> TriangleMesh::getFacesConnectedToVertex(SizeType vertexIndex) const {
	std::set<SizeType> facesIndices;

	for (SizeType f = 0u; f < _faces.size(); ++f) {
		for (SizeType i = 0u; i < 3u; ++i) {
			if (getFace(f).get(i) == vertexIndex) {
				facesIndices.insert(f);
				break;
			}
		}
	}

	return facesIndices; // TODO: make sure move operator is being called
}

std::set<SizeType> TriangleMesh::getVerticesConnectedToVertex(SizeType vertexIndex) const {
	std::set<SizeType> verticesIndices;

	for (SizeType f = 0u; f < _faces.size(); ++f) {
		for (SizeType i = 0u; i < 3u; ++i) {
			if (getFace(f).get(i) == vertexIndex) { // vertex i of face is vertexIndex
				verticesIndices.insert(getFace(f).get((i + 1) % 3)); // insert the two other vertices in the result
				verticesIndices.insert(getFace(f).get((i + 2) % 3));
				break;
			}
		}
	}

	return verticesIndices; // TODO: make sure move operator is being called
}

std::set<SizeType> TriangleMesh::getFaceNeighbors(SizeType faceIndex) const {
	std::set<SizeType> neighbors;

	for (auto i = SizeType(0); (neighbors.size() < SizeType(3)) && (i < getNumberOfFaces()); ++i) {
		if (i == faceIndex) {
			continue;
		}

		const auto sharedCount = SizeType(getFace(faceIndex).getSharedVertices(getFace(i)).size());

		if (sharedCount == 2u) {
			neighbors.insert(i);
		}
	}

	return neighbors;
}

std::array<UnorderedPair<SizeType>, 3u> TriangleMesh::getFaceEdges(SizeType faceIndex) const {
	const auto& face = getFace(faceIndex);

	const std::array<UnorderedPair<SizeType>, 3u> faceEdges = {
		UnorderedPair<SizeType>(face.getA(), face.getB()),
		UnorderedPair<SizeType>(face.getB(), face.getC()),
		UnorderedPair<SizeType>(face.getC(), face.getA())
	};

	return faceEdges;
}

std::set<UnorderedPair<SizeType> > TriangleMesh::getEdges() const {
	std::set<UnorderedPair<SizeType> > meshEdges;

	for (SizeType i = 0u; i < getNumberOfFaces(); ++i) {
		const auto faceEdges = getFaceEdges(i);
		meshEdges.insert(faceEdges.begin(), faceEdges.end());
	}

	return meshEdges;
}

SizeType TriangleMesh::findFace(SizeType va, SizeType vb, SizeType vc) const {
	assert(va != vb);
	assert(va != vc);
	assert(vb != vc);

	for (SizeType i = 0u; i < getNumberOfFaces(); ++i) {
		const auto& fi = getFace(i);

		if (fi.hasVertex(TriangularFace::IndexType(va)) && fi.hasVertex(TriangularFace::IndexType(vb)) && fi.hasVertex(TriangularFace::IndexType(vc))) {
			return i;
		}
	}

	return std::numeric_limits<SizeType>::max();
}

bool TriangleMesh::checkIntegrity() const {
	std::set<SizeType> referencedVertices;

	for (SizeType i = 0u; i < getNumberOfFaces(); ++i) {
		for (SizeType j = 0u; j < 3u; ++j) {
			const SizeType vIndex = getFace(i).get(j);

			if (vIndex >= getNumberOfVertices()) {
				return false; // face points to a vertex that we do not have
			}

			referencedVertices.insert(vIndex);
		}
	}

	return referencedVertices.size() == getNumberOfVertices();
}

void TriangleMesh::divideFaceInThreeByCentroid(SizeType faceIndex) {
	assert(faceIndex < getNumberOfFaces());

	TriangularFace& face = getFace(faceIndex);
	const Triangle faceTriangle = getFaceTriangle(face);

	// create the new vertex
	Vertex newVertex(faceTriangle.getCenter(), glm::vec3(0,0,0), glm::vec2(0,0));

	for (SizeType i = 0u; i < 3u; ++i) {
		newVertex.setNormal(newVertex.getNormal() + getVertex(face.get(i)).getNormal());
		newVertex.setUV(newVertex.getUV() + getVertex(face.get(i)).getUV());
	}

	newVertex.setNormal(newVertex.getNormal() / 3.0f);
	newVertex.setUV(newVertex.getUV() / 3.0f);

	const auto newVertexIndex = _vertices.size();
	
	// create the two new faces
	const TriangularFace newFaceA(TriangularFace::IndexType(newVertexIndex), face.getA(), face.getB());
	const TriangularFace newFaceB(TriangularFace::IndexType(newVertexIndex), face.getC(), face.getA());

	// modify the divided face
	face.setA(TriangularFace::IndexType(newVertexIndex));

	// add 2 new faces
	_faces.push_back(newFaceA);
	_faces.push_back(newFaceB);

	// add 1 new vertex
	_vertices.push_back(newVertex);
}

void TriangleMesh::rotate(const glm::quat& rotation, const glm::vec3& rotationCenter) {
	for (SizeType i = 0u; i < getNumberOfVertices(); ++i) {
		getVertex(i).setPosition((rotation * (getVertex(i).getPosition() - rotationCenter)) + rotationCenter);
	}
}
void TriangleMesh::rotate(const glm::quat& rotation) {
	rotate(rotation, getCenter());
}

void TriangleMesh::setPosition(const glm::vec3& newPosition, const glm::vec3& basePosition) {
	const glm::vec3 displacement = newPosition - basePosition;

	for (SizeType i = 0u; i < getNumberOfVertices(); ++i) {
		getVertex(i).setPosition(getVertex(i).getPosition() + displacement);
	}
}
void TriangleMesh::setPosition(const glm::vec3& newPosition) {
	setPosition(newPosition, getCenter());
}

TriangleMesh TriangleMesh::transformed(const glm::mat4& matrix) const {
	TriangleMesh copy(*this);
	copy.transform(matrix);
	return copy;
}

void TriangleMesh::transform(const glm::mat4& matrix) {
	for (auto& v : _vertices) {
		v.setPosition(glm::vec3(matrix * glm::vec4(v.getPosition(), 1.0f)));
		v.setNormal(glm::vec3(matrix * glm::vec4(v.getNormal(), 0.0f)));
	}
}

glm::vec3 TriangleMesh::getCenter() const {
	glm::vec3 center(0, 0, 0);

	if (SizeType numberOfVertices = getNumberOfVertices()) {
		for (SizeType i = 0u; i < numberOfVertices; ++i) {
			center += getVertex(i).getPosition();
		}

		center /= Float(numberOfVertices);
	}

	return center;
}

std::pair<Float, Float> TriangleMesh::getEdgeLengthStatistics() const {
	return getEdgeLengthStatistics(getEdges());
}

std::pair<Float, Float> TriangleMesh::getEdgeLengthStatistics(const std::set<UnorderedPair<SizeType> >& edges) const {
	const auto numberOfEdges = SizeType(edges.size());

	Float average = 0.0f;

	for (auto it = edges.begin(); it != edges.end(); ++it) {
		average += glm::distance(getVertex(it->first).getPosition(), getVertex(it->second).getPosition());
	}

	if (numberOfEdges > 0u) {
		average /= Float(numberOfEdges);
	}

	Float variance = 0.0f;

	for (auto it = edges.begin(); it != edges.end(); ++it) {
		const Float deviation = glm::distance(getVertex(it->first).getPosition(), getVertex(it->second).getPosition()) - average;
		variance += deviation * deviation;
	}

	if (numberOfEdges > 0u) {
		variance /= Float(numberOfEdges);
	}

	const Float standardDeviation = std::sqrt(variance);

	return std::make_pair(average, standardDeviation);
}

namespace {

struct _VertexData {
	Vertex v;
	std::set<SizeType> faces; // faces that point to this vertex
	SizeType newIndex = std::numeric_limits<SizeType>::max();
};

}

void TriangleMesh::removeLooseVertices() {
	std::vector<_VertexData> newVertices(_vertices.size());

	for (auto i = SizeType(0); i < SizeType(newVertices.size()); ++i) {
		newVertices[i].v = _vertices[i];
	}

	// register in each vertex the faces that point to it
	for (auto i = SizeType(0); i < SizeType(_faces.size()); ++i) {
		for (auto j = SizeType(0); j < SizeType(3); ++j) {
			newVertices[_faces[i].get(j)].faces.insert(i);
		}
	}

	// create new vertex vector, only with vertices that are referenced by at least one face
	_vertices.clear();

	for (auto i = SizeType(0); i < SizeType(newVertices.size()); ++i) {
		if (newVertices[i].faces.size()) {
			newVertices[i].newIndex = SizeType(_vertices.size()); // register new vertex index
			_vertices.push_back(newVertices[i].v);
		}
	}

	// update faces
	for (auto i = SizeType(0); i < SizeType(_faces.size()); ++i) {
		for (auto j = SizeType(0); j < SizeType(3); ++j) { // for each face vertex
			const auto newIndex = newVertices[_faces[i].get(j)].newIndex;

			if (newIndex < _vertices.size()) { // if vertex has new index
				_faces[i].set(j, TriangularFace::IndexType(newIndex));
			}
		}
	}
}

TriangleMesh::RayQueryResult TriangleMesh::rayQuery(const Ray& ray, bool sortResult) const {
	return rayQuery(LineSegment(ray.getOrigin(), ray.getFarPoint()), sortResult);
}

TriangleMesh::RayQueryResult TriangleMesh::rayQuery(const LineSegment& segment, bool sortResult) const {
	RayQueryResult result;

	for (SizeType i = 0u; i < getNumberOfFaces(); ++i) {
		const Triangle triangle = getFaceTriangle(i);
		glm::vec3 intersectionPoint;

		if (
			intersection::triangleLineSegment(triangle, segment, intersectionPoint) ||
			intersection::triangleLineSegment(Triangle(triangle.getA(), triangle.getC(), triangle.getB()), segment, intersectionPoint) // testing both sides of the triangle
		) {
			const Float distance2 = glm::length2(intersectionPoint - segment.getP());

			RayQueryData data;
			data.faceIndex = i;
			data.distance2 = distance2;

			result.push_back(data);
		}
	}

	if (sortResult) {
		std::sort(result.begin(), result.end());
	}

	return result;
}

}