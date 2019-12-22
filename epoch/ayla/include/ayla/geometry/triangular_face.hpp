#ifndef AYLA_TRIANGULAR_FACE_HPP
#define AYLA_TRIANGULAR_FACE_HPP

#include "ayla/config.hpp"

#include "ayla/serialization/triangular_face_serializer.hpp"

#include <cstdint>
#include <set>

namespace ayla {

/**
 * The indices of three vertices in a triangle mesh.
 * It tells you how to "connect the dots" to create a face.
 *
 * @see ayla::Vertex
 * @see ayla::TriangleMesh
 *
 * @author O Programador
 */
class AYLA_API TriangularFace final {
public:
	using IndexType = std::uint32_t; // must have 32-bits

public:
	TriangularFace();
	TriangularFace(IndexType va, IndexType vb, IndexType vc);

	bool hasVertex(IndexType vIndex) const;
	bool hasEdge(IndexType v0Index, IndexType v1Index) const;

	std::set<IndexType> getSharedVertices(const TriangularFace& other) const;
	std::set<IndexType> getUnsharedVertices(const TriangularFace& other) const;

	IndexType get(SizeType i) const;

	inline IndexType getA() const { return _va; }
	inline IndexType getB() const { return _vb; }
	inline IndexType getC() const { return _vc; }

	inline void setA(IndexType v) { _va = v; }
	inline void setB(IndexType v) { _vb = v; }
	inline void setC(IndexType v) { _vc = v; }

	void set(SizeType i, IndexType v);

	void flip();

private:
	IndexType _va = std::numeric_limits<IndexType>::max();
	IndexType _vb = std::numeric_limits<IndexType>::max();
	IndexType _vc = std::numeric_limits<IndexType>::max();

	template<class Archive> friend void boost::serialization::serialize(Archive&, ayla::TriangularFace&, const unsigned int);
};

}

#endif // AYLA_TRIANGULAR_FACE_HPP