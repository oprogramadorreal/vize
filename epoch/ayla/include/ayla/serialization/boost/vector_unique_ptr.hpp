/**
 * Serialize a std::vector of std::unique_ptr.
 *
 * Do NOT include <boost/serialization/vector.hpp>
 *
 * http://stackoverflow.com/questions/13347776/boost-serialization-of-stl-collection-of-std-unique-ptrs
 */

#ifndef AYLA_VECTOR_UNIQUE_PTR_HPP
#define AYLA_VECTOR_UNIQUE_PTR_HPP

#include <vector>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/collection_size_type.hpp>

namespace boost {
namespace serialization {

template<class Archive, class T, class Allocator>
void save(Archive& ar, const std::vector<T, Allocator>& t, const unsigned int) {
	collection_size_type count(t.size());
	ar << BOOST_SERIALIZATION_NVP(count);

	for (auto it = t.begin(), end = t.end(); it != end; ++it) {
		ar << boost::serialization::make_nvp("item", (*it));
	}
}

template<class Archive, class T, class Allocator>
void load(Archive& ar, std::vector<T, Allocator>& t, const unsigned int) {
	collection_size_type count;
	ar >> BOOST_SERIALIZATION_NVP(count);
	t.clear();
	t.reserve(count);

	while (count-- > 0) {
		T i;
		ar >> boost::serialization::make_nvp("item", i);
		t.push_back(std::move(i)); // use std::move
	}
}

template<class Archive, class T, class Allocator>
void serialize(Archive& ar, std::vector<T, Allocator>& t, const unsigned int file_version) {
	boost::serialization::split_free(ar, t, file_version);
}

}
}

#endif // AYLA_VECTOR_UNIQUE_PTR_HPP