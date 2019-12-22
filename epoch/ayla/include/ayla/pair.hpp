#ifndef AYLA_PAIR_HPP
#define AYLA_PAIR_HPP

#include <functional>

namespace ayla {

/**
 * For any comparable elements A and B, Pair(A,B) and Pair(B,A) represents different pairs.
 *
 * @see UnorderedPair
 * @see http://en.cppreference.com/w/cpp/concept/Compare
 *
 * @author O Programador
 */
template <class T, class COMPARE = std::less<T> >
class Pair {
public:
	Pair(const T& a, const T& b)
		: first(a), second(b)
	{ }

	bool operator<(const Pair& other) const {
		if (COMPARE()(first, other.first)) {
			return true;
		}

		if (COMPARE()(other.first, first)) {
			return false;
		}

		return COMPARE()(second, other.second);
	}

	bool operator==(const Pair& other) const {
		return !((*this) < other) && !(other < (*this));
	}

	bool operator!=(const Pair& other) const {
		return !(*this == other);
	}

	Pair& operator=(const Pair& other) {
		const_cast<T&>(first) = other.first;
		const_cast<T&>(second) = other.second;
		return *this;
	}

public:
	const T first;
	const T second;
};

/**
 * For any comparable elements A and B, UnorderedPair(A,B) and UnorderedPair(B,A) represents the same pair.
 *
 * @see Pair
 *
 * @author O Programador
 */
template <class T, class COMPARE = std::less<T> >
class UnorderedPair : public Pair<T, COMPARE> {
public:
	UnorderedPair(const T& a, const T& b)
		: Pair<T, COMPARE>(
			COMPARE()(a,b) ? a : b,
			COMPARE()(a,b) ? b : a
		)
	{ }
};

}

#endif // AYLA_PAIR_HPP
