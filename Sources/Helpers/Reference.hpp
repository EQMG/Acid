#pragma once

#include "StdAfx.hpp"

namespace acid
{
template<typename T>
class Reference
{
public:
	Reference() noexcept :
		m_reference(nullptr)
	{
	}

	Reference(T &reference) noexcept :
		m_reference(std::addressof(reference))
	{
	}

	~Reference() = default;

	// Access the stored reference
	T &get() const noexcept { return *m_reference; }

	// Access the stored reference
	constexpr operator T &() const noexcept { return *m_reference; }

	// Get the address of the stored reference
	T *operator&() const noexcept { return m_reference; }

	// Call the function of the stored reference
	T *operator->() const noexcept { return m_reference; }

	// Call the stored function
	template<typename... Args>
	std::invoke_result_t<T &, Args...> operator()(Args &&...args) const
	{
		return std::invoke(get(), std::forward<Args>(args)...);
	}

	bool operator==(const Reference &other) const noexcept
	{
		return m_reference == other.m_reference;
	}

	bool operator!=(const Reference &other) const noexcept
	{
		return !(*this == other);
	}

private:
	// Address of the referenced object
	T *m_reference;
};
}
