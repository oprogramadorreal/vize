#ifndef LUCCA_API_HPP
#define LUCCA_API_HPP

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(__CYGWIN__)
	#if defined(LUCCA_LIB)
		#define LUCCA_API __declspec(dllexport)
	#else
		#define LUCCA_API __declspec(dllimport)
	#endif
#endif

#ifndef LUCCA_API
	#define LUCCA_API
#endif

#endif // LUCCA_API_HPP