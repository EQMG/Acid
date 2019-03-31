#pragma once

#include "StdAfx.hpp"

namespace acid
{
/// <summary>
/// A class that holds a future and it's value that will update as future gets new valid values.
/// </summary>
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

private:
	std::future<T> m_future;
	std::optional<T> m_current;
};
}
