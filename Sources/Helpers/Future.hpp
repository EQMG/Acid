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
	Future() = default;

	Future(std::future<T> future) :
		m_future(std::move(future))
	{
	}

	bool HasValue() const
	{
		return m_future.valid() || m_current;
	}

	T &Get()
	{
		if (m_future.valid())
		{
			m_current = m_future.get();
		}

		return *m_current;
	}
	
	constexpr operator bool() const
	{
		return HasValue();
	}

	T &operator*() { return Get(); }

	T *operator->() { return &Get(); }

private:
	std::future<T> m_future;
	std::optional<T> m_current;
};
}
