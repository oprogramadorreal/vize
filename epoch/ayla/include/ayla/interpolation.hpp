#ifndef AYLA_INTERPOLATION_HPP
#define AYLA_INTERPOLATION_HPP

#include "ayla/config.hpp"

namespace ayla {

/**
 * @param a First value.
 * @param b Second value.
 * @param x Desired interpolation position. Must be between 0 and 1.
 *
 * @return Linear interpolation between @param a and @param b at @param x position.
 *
 * @see https://en.wikipedia.org/wiki/Linear_interpolation
 */
template <class T>
T linearInterpolation(T a, T b, Float x) {
	assert(x >= Float(0.0f));
	assert(x <= Float(1.0f));

	return T(a * (Float(1.0f) - x) + b * x);
}

/**
 * @param a First value.
 * @param b Second value.
 * @param x Desired interpolation position.
 * @param x0 Minimum x value.
 * @param x1 Maximum x value.
 *
 * @return Linear interpolation between @param a and @param b at @param x position.
 *
 * @see https://en.wikipedia.org/wiki/Linear_interpolation
 */
template <class T>
T linearInterpolation(T a, T b, Float x, Float x0, Float x1) {
	assert(x0 < x1);
	assert(x >= x0);
	assert(x <= x1);

	x = (x - x0) / (x1 - x0); // normalize x

	return linearInterpolation(a, b, x);
}

}

#endif // AYLA_INTERPOLATION_HPP