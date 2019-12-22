#ifndef AYLA_EXPLICIT_INSTANTIATION_MACROS_HPP
#define AYLA_EXPLICIT_INSTANTIATION_MACROS_HPP

#if defined(_MSC_VER)
	#pragma warning( push )
	#pragma warning( disable : 4244 ) // conversion from 'std::streamsize' to 'size_t', possible loss of data
#endif

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#if defined(_MSC_VER)
	#pragma warning( pop )
#endif

#define EMPTY_DECLSPEC_API

#define EXPLICIT_INSTANTIATION_SERIALIZE_FUNC(Type) EXPLICIT_INSTANTIATION_SERIALIZE_FUNC_P(Type, EMPTY_DECLSPEC_API)

#define EXPLICIT_INSTANTIATION_SERIALIZE_FUNC_P(Type, DECLSPEC_API) \
   template DECLSPEC_API void serialize<boost::archive::binary_oarchive>(boost::archive::binary_oarchive&, Type&, const unsigned int); \
   template DECLSPEC_API void serialize<boost::archive::binary_iarchive>(boost::archive::binary_iarchive&, Type&, const unsigned int); \
   template DECLSPEC_API void serialize<boost::archive::text_oarchive>(boost::archive::text_oarchive&, Type&, const unsigned int); \
   template DECLSPEC_API void serialize<boost::archive::text_iarchive>(boost::archive::text_iarchive&, Type&, const unsigned int); \
   template DECLSPEC_API void serialize<boost::archive::xml_oarchive>(boost::archive::xml_oarchive&, Type&, const unsigned int); \
   template DECLSPEC_API void serialize<boost::archive::xml_iarchive>(boost::archive::xml_iarchive&, Type&, const unsigned int);

#define EXPLICIT_INSTANTIATION_SERIALIZE_METHOD(ClassName) EXPLICIT_INSTANTIATION_SERIALIZE_METHOD_P(ClassName, EMPTY_DECLSPEC_API)

#define EXPLICIT_INSTANTIATION_SERIALIZE_METHOD_P(ClassName, DECLSPEC_API) \
   template DECLSPEC_API void ClassName::serialize<boost::archive::binary_oarchive>(boost::archive::binary_oarchive&, const unsigned int); \
   template DECLSPEC_API void ClassName::serialize<boost::archive::binary_iarchive>(boost::archive::binary_iarchive&, const unsigned int); \
   template DECLSPEC_API void ClassName::serialize<boost::archive::text_oarchive>(boost::archive::text_oarchive&, const unsigned int); \
   template DECLSPEC_API void ClassName::serialize<boost::archive::text_iarchive>(boost::archive::text_iarchive&, const unsigned int); \
   template DECLSPEC_API void ClassName::serialize<boost::archive::xml_oarchive>(boost::archive::xml_oarchive&, const unsigned int); \
   template DECLSPEC_API void ClassName::serialize<boost::archive::xml_iarchive>(boost::archive::xml_iarchive&, const unsigned int)

#define EXPLICIT_INSTANTIATION_SAVE_CONSTRUCT_DATA_FUNC(Type) EXPLICIT_INSTANTIATION_SAVE_CONSTRUCT_DATA_FUNC_P(Type, EMPTY_DECLSPEC_API)

#define EXPLICIT_INSTANTIATION_SAVE_CONSTRUCT_DATA_FUNC_P(Type, DECLSPEC_API) \
   template DECLSPEC_API void save_construct_data<boost::archive::binary_oarchive>(boost::archive::binary_oarchive&, const Type*, const unsigned int); \
   template DECLSPEC_API void save_construct_data<boost::archive::binary_iarchive>(boost::archive::binary_iarchive&, const Type*, const unsigned int); \
   template DECLSPEC_API void save_construct_data<boost::archive::text_oarchive>(boost::archive::text_oarchive&, const Type*, const unsigned int); \
   template DECLSPEC_API void save_construct_data<boost::archive::text_iarchive>(boost::archive::text_iarchive&, const Type*, const unsigned int); \
   template DECLSPEC_API void save_construct_data<boost::archive::xml_oarchive>(boost::archive::xml_oarchive&, const Type*, const unsigned int); \
   template DECLSPEC_API void save_construct_data<boost::archive::xml_iarchive>(boost::archive::xml_iarchive&, const Type*, const unsigned int)

#define EXPLICIT_INSTANTIATION_LOAD_CONSTRUCT_DATA_FUNC(Type) EXPLICIT_INSTANTIATION_LOAD_CONSTRUCT_DATA_FUNC_P(Type, EMPTY_DECLSPEC_API)

#define EXPLICIT_INSTANTIATION_LOAD_CONSTRUCT_DATA_FUNC_P(Type, DECLSPEC_API) \
   template DECLSPEC_API void load_construct_data<boost::archive::binary_oarchive>(boost::archive::binary_oarchive&, Type*, const unsigned int); \
   template DECLSPEC_API void load_construct_data<boost::archive::binary_iarchive>(boost::archive::binary_iarchive&, Type*, const unsigned int); \
   template DECLSPEC_API void load_construct_data<boost::archive::text_oarchive>(boost::archive::text_oarchive&, Type*, const unsigned int); \
   template DECLSPEC_API void load_construct_data<boost::archive::text_iarchive>(boost::archive::text_iarchive&, Type*, const unsigned int); \
   template DECLSPEC_API void load_construct_data<boost::archive::xml_oarchive>(boost::archive::xml_oarchive&, Type*, const unsigned int); \
   template DECLSPEC_API void load_construct_data<boost::archive::xml_iarchive>(boost::archive::xml_iarchive&, Type*, const unsigned int)

#define EXPLICIT_INSTANTIATION_SAVE_FUNC(Type) EXPLICIT_INSTANTIATION_SAVE_FUNC_P(Type, EMPTY_DECLSPEC_API)

#define EXPLICIT_INSTANTIATION_SAVE_FUNC_P(Type, DECLSPEC_API) \
   template DECLSPEC_API void save<boost::archive::binary_oarchive>(boost::archive::binary_oarchive&, const Type&, const unsigned int); \
   template DECLSPEC_API void save<boost::archive::text_oarchive>(boost::archive::text_oarchive&, const Type&, const unsigned int); \
   template DECLSPEC_API void save<boost::archive::xml_oarchive>(boost::archive::xml_oarchive&, const Type&, const unsigned int)

#define EXPLICIT_INSTANTIATION_LOAD_FUNC(Type) EXPLICIT_INSTANTIATION_LOAD_FUNC_P(Type, EMPTY_DECLSPEC_API)

#define EXPLICIT_INSTANTIATION_LOAD_FUNC_P(Type, DECLSPEC_API) \
   template DECLSPEC_API void load<boost::archive::binary_iarchive>(boost::archive::binary_iarchive&, Type&, const unsigned int); \
   template DECLSPEC_API void load<boost::archive::text_iarchive>(boost::archive::text_iarchive&, Type&, const unsigned int); \
   template DECLSPEC_API void load<boost::archive::xml_iarchive>(boost::archive::xml_iarchive&, Type&, const unsigned int)

#endif // AYLA_EXPLICIT_INSTANTIATION_MACROS_HPP
