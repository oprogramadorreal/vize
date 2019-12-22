#ifndef AYLA_API_HPP
#define AYLA_API_HPP

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(__CYGWIN__)
	#if defined(AYLA_LIB)
		#define AYLA_API __declspec(dllexport)
	#else
		#define AYLA_API __declspec(dllimport)
	#endif
#endif

#ifndef AYLA_API
	#define AYLA_API
#endif

#endif // AYLA_API_HPP
