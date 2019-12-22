#ifndef LUCCA_QT_API_HPP
#define LUCCA_QT_API_HPP

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(__CYGWIN__)
	#if defined(LUCCA_QT_LIB)
		#define LUCCA_QT_API __declspec(dllexport)
	#else
		#define LUCCA_QT_API __declspec(dllimport)
	#endif
#endif

#ifndef LUCCA_QT_API
	#define LUCCA_QT_API
#endif

#endif // LUCCA_QT_API_HPP