#include "ayla/geometry/triangular_face.hpp"

namespace ayla {

TriangularFace::TriangularFace() = default;

TriangularFace::TriangularFace(IndexType va, IndexType vb, IndexType vc)
	: _va(va), _vb(vb), _vc(vc)
{
	assert(_va != _vb);
	assert(_va != _vc);
	assert(_vb != _vc);
}

bool TriangularFace::hasVertex(IndexType vIndex) const {
	return (vIndex == _va) || (vIndex == _vb) || (vIndex == _vc);
}

bool TriangularFace::hasEdge(IndexType v0Index, IndexType v1Index) const {
	return hasVertex(v0Index) && hasVertex(v1Index);
}

TriangularFace::IndexType TriangularFace::get(SizeType i) const {
	assert(i < 3u);
	return (&_va)[i];
}

void TriangularFace::set(SizeType i, IndexType v) {
	assert(i < 3u);
	(&_va)[i] = v;
}

void TriangularFace::flip() {
	std::swap(_vb, _vc);
}

std::set<TriangularFace::IndexType> TriangularFace::getSharedVertices(const TriangularFace& other) const {
	std::set<IndexType> sharedVertices;

	for (auto i = SizeType(0); i < SizeType(3); ++i) {
		if (hasVertex(other.get(i))) {
			sharedVertices.insert(other.get(i));
		}
	}

	return sharedVertices;
}

std::set<TriangularFace::IndexType> TriangularFace::getUnsharedVertices(const TriangularFace& other) const {
	std::set<IndexType> unsharedVertices;

	for (auto i = SizeType(0); i < SizeType(3); ++i) {
		if (!hasVertex(other.get(i))) {
			unsharedVertices.insert(other.get(i));
		}

		if (!other.hasVertex(get(i))) {
			unsharedVertices.insert(get(i));
		}
	}

	return unsharedVertices;
}

}