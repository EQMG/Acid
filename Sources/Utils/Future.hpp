#pragma once

#include <future>
#include <optional>

namespace acid {
/**
 * @brief Class that holds a future and it's value that will update as future gets new valid values.
 * @tparam T The type to hold.
 */
template<typename T>
class Future {
public:
	Future() noexcept = default;

	Future(std::future<T> &&future) noexcept :
		future(std::move(future)) {
	}

	bool has_value() const noexcept {
		return future.valid() || current;
	}

	T &get() noexcept {
		if (future.valid()) {
			current = future.get();
		}

		return *current;
	}

	constexpr explicit operator bool() const noexcept { return has_value(); }
	constexpr operator T &() const noexcept { return *get(); }

	T &operator*() noexcept { return get(); }
	T &operator->() noexcept { return get(); }

	bool operator==(const Future &other) const noexcept {
		return future == other.future && current == other.current;
	}

	bool operator!=(const Future &other) const noexcept {
		return !operator==(other);
	}

private:
	std::future<T> future;
	std::optional<T> current;
};
}
