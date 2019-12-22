#include "ayla/geometry/index_3d.hpp"

namespace ayla {

namespace {
	const Index3D::ValueType _invalidValue = std::numeric_limits<Index3D::ValueType>::max();
}

Index3D::Index3D()
	: Index3D(_invalidValue)
{ }

Index3D::Index3D(ValueType ijk)
	: Index3D(ijk, ijk, ijk)
{ }

Index3D::Index3D(ValueType i, ValueType j, ValueType k) {
	_indices[0] = i;
	_indices[1] = j;
	_indices[2] = k;
}

bool Index3D::isValid() const {
	return (getI() != _invalidValue) &&
		(getJ() != _invalidValue) &&
		(getK() != _invalidValue);
}

Index3D::ValueType Index3D::getOneDimensionalIndex(SizeType iSize, SizeType jSize) const {
	return getI() +
		getJ() * Index3D::ValueType(iSize) +
		getK() * Index3D::ValueType(iSize * jSize);
}

Index3D::ValueType Index3D::getI() const {
	return _indices[0];
}

Index3D::ValueType Index3D::getJ() const {
	return _indices[1];
}

Index3D::ValueType Index3D::getK() const {
	return _indices[2];
}

Index3D::ValueType Index3D::operator[](SizeType index) const {
	assert(index < SizeType(3));
	return _indices[index];
}

Index3D::ValueType& Index3D::operator[](SizeType index) {
	assert(index < SizeType(3));
	return _indices[index];
}

bool Index3D::operator==(const Index3D& otherIndex) const {
	return (_indices[0] == otherIndex[0]) &&
		(_indices[1] == otherIndex[1]) &&
		(_indices[2] == otherIndex[2]);
}

bool Index3D::operator!=(const Index3D& otherIndex) const {
	return !(*this == otherIndex);
}

Index3D Index3D::operator+(const Index3D& otherIndex) const {
	return Index3D(
		_indices[0] + otherIndex[0],
		_indices[1] + otherIndex[1],
		_indices[2] + otherIndex[2]
	);
}

}