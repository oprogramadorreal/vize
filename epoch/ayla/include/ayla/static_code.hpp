#ifndef AYLA_STATIC_CODE_HPP
#define AYLA_STATIC_CODE_HPP

/**
 * Usage:
 *
 * AYLA_STATIC_CODE ( 
 *    // code code code
 *    // code code code
 * )
 *
 */

#define AYLA_STATIC_CODE__(a,b) a##b
#define AYLA_STATIC_CODE_(b,...) struct AYLA_STATIC_CODE__(once_struct,b) { AYLA_STATIC_CODE__(once_struct,b)() { __VA_ARGS__ } } AYLA_STATIC_CODE__(once_instance,b);
#define AYLA_STATIC_CODE(...) namespace { AYLA_STATIC_CODE_(__COUNTER__,__VA_ARGS__) }

#endif // AYLA_STATIC_CODE_HPP
