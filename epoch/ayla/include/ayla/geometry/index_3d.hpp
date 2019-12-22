#ifndef AYLA_INDEX_3D_HPP
#define AYLA_INDEX_3D_HPP

#include "ayla/config.hpp"

#include <array>
#include <cstdint>

namespace ayla {

/**
 * The index of an element in a generic three-dimensional matrix structure.
 *
 * @see ayla::RegularGridIndexer
 *
 * @author O Programador
 */
class AYLA_API Index3D final {
public:
	/**
	 * The actual integer type of the stored indices.
	 */
	using ValueType = std::int64_t;

public:
	/**
	 * Creates an invalid index.
	 */
	Index3D();

	explicit Index3D(ValueType ijk);

	Index3D(ValueType i, ValueType j, ValueType k = ValueType(0));

	bool isValid() const;

	/**
	 * Converts this 3D index into an unidimensional value.
	 * This is useful if you store all your data in a single unidimensional array.
	 */
	ValueType getOneDimensionalIndex(SizeType iSize, SizeType jSize) const;

	ValueType getI() const;
	ValueType getJ() const;
	ValueType getK() const;

	ValueType operator[](SizeType index) const;
	ValueType& operator[](SizeType index);

	bool operator==(const Index3D& otherIndex) const;
	bool operator!=(const Index3D& otherIndex) const;

	Index3D operator+(const Index3D& otherIndex) const;

private:
	std::array<ValueType, 3> _indices;
};

}

#endif // AYLA_INDEX_3D_HPP