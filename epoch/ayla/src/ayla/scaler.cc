#include "ayla/scaler.hpp"

namespace ayla {

template <class T>
_Scaler<T>::_Scaler(const T &a, const T &b, const T &u, const T &v)
	: _a(a), _b(b), _u(u), _v(v)
{ }

template <class T>
T _Scaler<T>::scale(const T& xInAb) const {
	return (xInAb - _a) / (_b - _a) * (_v - _u) + _u;
}

template class _Scaler<Float>;

}