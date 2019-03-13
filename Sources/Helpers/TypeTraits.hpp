#pragma once

#include "AcidPch.hpp"

namespace acid
{
/// <summary>
/// A
/// helper
/// for
/// checking
/// types.
/// </summary>
class ACID_EXPORT TypeTraits
{
  public:
	template<typename T>
	struct is_optional : std::false_type
	{
	};

	template<typename T>
	struct is_optional<std::optional<T>> : std::true_type
	{
	};

	template<typename T>
	struct is_pair : std::false_type
	{
	};

	template<typename T, typename U>
	struct is_pair<std::pair<T, U>> : std::true_type
	{
	};

	template<typename T>
	struct is_vector : std::false_type
	{
	};

	template<typename T, typename A>
	struct is_vector<std::vector<T, A>> : std::true_type
	{
	};

	template<typename T, typename U = void>
	struct is_map : std::false_type
	{
	};

	template<typename T>
	struct is_map<T, std::void_t<typename T::key_type, typename T::mapped_type, decltype(std::declval<T&>()[std::declval<const typename T::key_type&>()])>> : std::true_type
	{
	};

	template<typename T>
	static T* AsPtr(T& obj)
	{
		return &obj;
	}

	template<typename T>
	static T* AsPtr(T* obj)
	{
		return obj;
	}

	template<typename T>
	static T* AsPtr(const std::shared_ptr<T>& obj)
	{
		return obj.get();
	}

	template<typename T>
	static T* AsPtr(const std::unique_ptr<T>& obj)
	{
		return obj.get();
	}
};
}
