#ifndef LUCCA_ENUM_FLAGS_HPP
#define LUCCA_ENUM_FLAGS_HPP

#include "lucca/config.hpp"

namespace lucca {

/**
 * @author O Programador
 */
template <class EnumType>
class EnumFlags final {
public:
	bool testFlag(EnumType value) const {
		const auto valueFlag = static_cast<FlagsType>(value);

		return (_flags & valueFlag) == valueFlag
			&& (valueFlag != FlagsType(0) || _flags == valueFlag);
	}

	void addFlag(EnumType value) {
		_flags |= FlagsType(value);
	}

	void removeFlag(EnumType value) {
		_flags &= ~value;
	}

	void clearFlags() {
		_flags = FlagsType(0);
	}

private:
	using FlagsType = std::int32_t;
	FlagsType _flags = FlagsType(0);
};

}

#endif // LUCCA_ENUM_FLAGS_HPP