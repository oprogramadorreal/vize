#ifndef AYLA_FLOAT_HPP
#define AYLA_FLOAT_HPP

#include <cstdlib>

namespace ayla {

using Float = float;
using Double = double;

/**
 * @return TRUE if @param a is close enough to zero.
 */
inline bool isZero(Float a, Float epsilon = std::numeric_limits<Float>::epsilon()) {
	return std::abs(a) <= epsilon;
}

}

#endif // AYLA_FLOAT_HPP