#ifndef AYLA_GL_API_HPP
#define AYLA_GL_API_HPP

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(__CYGWIN__)
	#if defined(AYLA_GL_LIB)
		#define AYLA_GL_API __declspec(dllexport)
	#else
		#define AYLA_GL_API __declspec(dllimport)
	#endif
#endif

#ifndef AYLA_GL_API
	#define AYLA_GL_API
#endif

#endif // AYLA_GL_API_HPP