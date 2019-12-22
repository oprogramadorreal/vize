#ifndef AYLA_SCALER_HPP
#define AYLA_SCALER_HPP

#include "ayla/config.hpp"

namespace ayla {

/**
 * It converts a value 'x' in the scale going from 'a' to 'b'
 * into the corresponding value in the scale going from 'u' to 'v'.
 * 
 * @author O Programador
 */
template <class T>
class AYLA_API _Scaler final {
public:
	_Scaler(const T &a, const T &b, const T &u, const T &v);

	/**
	 * @param xInAb A value in ab scale.
	 * @return A value in uv scale.
	 */
	T scale(const T& xInAb) const;

private:
	T _a, _b; // source scale
	T _u, _v; // target scale
};

using FloatScaler = _Scaler<Float>;

}

#endif // AYLA_SCALER_HPP