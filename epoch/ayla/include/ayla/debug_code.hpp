#ifndef AYLA_DEBUG_CODE_HPP
#define AYLA_DEBUG_CODE_HPP

#if defined(DEBUG) || defined(_DEBUG)
	#include <iostream>
	#define AYLA_DEBUG_CODE(x) ( [&](){ x; }() );
#else
	#define AYLA_DEBUG_CODE(x) (void());
#endif

#define AYLA_DEBUG_MESSAGE(message) AYLA_DEBUG_CODE(std::cout << message << std::endl)

#endif // AYLA_DEBUG_CODE_HPP
