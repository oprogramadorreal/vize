#ifndef AYLA_SIGN_HPP
#define AYLA_SIGN_HPP

namespace ayla {

/**
 * http://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
 */
template <typename T> int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

}

#endif // AYLA_SIGN_HPP