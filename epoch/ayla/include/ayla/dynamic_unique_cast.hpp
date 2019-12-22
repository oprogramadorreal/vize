#ifndef AYLA_DYNAMIC_UNIQUE_CAST_HPP
#define AYLA_DYNAMIC_UNIQUE_CAST_HPP

#include <memory>

namespace ayla {

/**
 * @warning This will remove the ownership from @param pointer.
 *
 * @author O Programador
 */
template <class T, class U>
std::unique_ptr<T> dynamic_unique_cast(std::unique_ptr<U> pointer) {
	if (pointer) {
		if (auto rawResult = dynamic_cast<T*>(pointer.get())) {
			std::unique_ptr<T> result(rawResult);
			pointer.release();

			return std::move(result);
		}
	}

	return nullptr;
}

}

#endif // AYLA_DYNAMIC_UNIQUE_CAST_HPP