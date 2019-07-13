#pragma once

#include "Node.hpp"

namespace acid
{
template<typename T>
T NodeReturn::Get()
{
	if (!has_value())
	{
		return {};
	}

	return m_value->Get<T>();
}

template<typename T>
T NodeReturn::Get(const T &fallback)
{
	if (!has_value())
	{
		return {};
	}

	return m_value->Get<T>(fallback);
}

template<typename T>
void NodeReturn::Get(T &dest)
{
	if (!has_value())
	{
		return;
	}

	return m_value->Get<T>(dest);
}

template<typename T, typename K>
void NodeReturn::Get(T &dest, const K &fallback)
{
	if (!has_value())
	{
		return;
	}

	return m_value->Get<T>(dest, fallback);
}

template<typename T>
void NodeReturn::Set(const T &value)
{
	return get()->Set<T>(value);
}

template<typename T>
Node & NodeReturn::operator=(const T &rhs)
{
	return *get() = rhs;
}
}
