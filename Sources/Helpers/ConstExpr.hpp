#pragma once

#include "StdAfx.hpp"

namespace acid {
template<typename T>
struct is_optional : std::false_type {
};

template<typename T>
struct is_optional<std::optional<T>> : std::true_type {
};

template<typename T>
inline constexpr bool is_optional_v = is_optional<T>::value;

template<typename T>
struct is_pair : std::false_type {
};

template<typename T, typename U>
struct is_pair<std::pair<T, U>> : std::true_type {
};

template<typename T>
inline constexpr bool is_pair_v = is_pair<T>::value;

template<typename T>
struct is_vector : std::false_type {
};

template<typename T, typename A>
struct is_vector<std::vector<T, A>> : std::true_type {
};

template<typename T>
inline constexpr bool is_vector_v = is_vector<T>::value;

template<typename T, typename U = void>
struct is_map : std::false_type {
};

template<typename T>
struct is_map<T, std::void_t<typename T::key_type, typename T::mapped_type, decltype(std::declval<T &>()[std::declval<const typename T::key_type &>()])>> : std::true_type {
};

template<typename T>
inline constexpr bool is_map_v = is_map<T>::value;

template<typename T>
struct is_unique_ptr : std::false_type {
};

template<typename T, typename D>
struct is_unique_ptr<std::unique_ptr<T, D>> : std::true_type {
};

template<typename T>
inline constexpr bool is_unique_ptr_v = is_unique_ptr<T>::value;

template<typename T>
struct is_shared_ptr : std::false_type {
};

template<typename T>
struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {
};

template<typename T>
inline constexpr bool is_shared_ptr_v = is_shared_ptr<T>::value;

template<typename T>
struct is_weak_ptr : std::false_type {
};

template<typename T>
struct is_weak_ptr<std::weak_ptr<T>> : std::true_type {
};

template<typename T>
inline constexpr bool is_weak_ptr_v = is_weak_ptr<T>::value;

template<typename T>
inline constexpr bool is_ptr_access_v = std::is_pointer_v<T> || is_unique_ptr_v<T> || is_shared_ptr_v<T> || is_weak_ptr_v<T>;

/**
 * @brief Helper header that contains constant expressions.
 */
class ACID_EXPORT ConstExpr {
public:
	// TODO C++20: std::to_address
	template<typename T>
	static T *AsPtr(T &obj) noexcept { return &obj; }

	template<typename T>
	static T *AsPtr(T *obj) noexcept { return obj; }

	template<typename T>
	static T *AsPtr(const std::shared_ptr<T> &obj) noexcept { return obj.get(); }

	template<typename T>
	static T *AsPtr(const std::unique_ptr<T> &obj) noexcept { return obj.get(); }

	template<typename T>
	static const T &AsRef(T &obj) noexcept { return obj; }

	template<typename T>
	static const T &AsRef(T *obj) noexcept { return *obj; }

	template<typename T>
	static const T &AsRef(const std::shared_ptr<T> &obj) noexcept { return *obj.get(); }

	template<typename T>
	static const T &AsRef(const std::unique_ptr<T> &obj) noexcept { return *obj.get(); }
};
}
