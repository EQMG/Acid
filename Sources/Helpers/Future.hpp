#pragma once

#include "StdAfx.hpp"

namespace acid
{
/**
 * @brief Class that holds a future and it's value that will update as future gets new valid values.
 * @tparam T The type to hold.
 */
template<typename T>
class Future
{
public:
	Future() noexcept = default;

	Future(std::future<T> &&future) noexcept :
		m_future(std::move(future))
	{
	}

	bool has_value() const noexcept
	{
		return m_future.valid() || m_current;
	}

	T &get() noexcept
	{
		if (m_future.valid())
		{
			m_current = m_future.get();
		}

		return *m_current;
	}

	constexpr explicit operator bool() const noexcept { return has_value(); }

	constexpr operator T &() const noexcept { return *get(); }

	T &operator*() noexcept { return get(); }

	T &operator->() noexcept { return get(); }

	bool operator==(const Future &other) const noexcept
	{
		return m_future == other.m_future && m_current == other.m_current;
	}

	bool operator!=(const Future &other) const noexcept
	{
		return !(*this == other);
	}

private:
	std::future<T> m_future;
	std::optional<T> m_current;
};
}
