#pragma once

#include <mutex>
#include "StdAfx.hpp"

namespace acid
{
template<typename>
class Delegate;

class ACID_EXPORT Observer
{
public:
	Observer() :
		m_deleted(false)
	{
	}

	virtual ~Observer()
	{
		m_deleted = true;
	}

	bool m_deleted;
};

template<typename TReturnType, typename... TArgs>
class Invoker
{
public:
	using ReturnType = std::vector<TReturnType>;

	static ReturnType Invoke(Delegate<TReturnType(TArgs ...)> &delegate, TArgs ... params)
	{
		std::lock_guard<std::mutex> lock(delegate.m_mutex);
		ReturnType returnValues;

		for (auto it = delegate.m_functions.begin(); it != delegate.m_functions.end();)
		{
			if (it->IsDeleted())
			{
				it = delegate.m_functions.erase(it);
				continue;
			}

			returnValues.emplace_back((*it->function)(params...));
			++it;
		}

		return returnValues;
	}
};

template<typename... TArgs>
class Invoker<void, TArgs...>
{
public:
	using ReturnType = void;

	static void Invoke(Delegate<void(TArgs ...)> &delegate, TArgs ... params)
	{
		std::lock_guard<std::mutex> lock(delegate.m_mutex);

		if (delegate.m_functions.empty())
		{
			return;
		}

		for (auto it = delegate.m_functions.begin(); it != delegate.m_functions.end();)
		{
			if (it->IsDeleted())
			{
				it = delegate.m_functions.erase(it);
				continue;
			}

			it->function(params...);
			++it;
		}
	}
};

template<typename TReturnType, typename... TArgs>
class Delegate<TReturnType(TArgs ...)>
{
public:
	using Invoker = acid::Invoker<TReturnType, TArgs...>;
	using FunctionType = std::function<TReturnType(TArgs ...)>;
	using FunctionObserver = std::reference_wrapper<Observer>;
	struct FunctionPair
	{
		FunctionType function;
		std::vector<FunctionObserver> observers;

		bool IsDeleted()
		{
			for (const auto &observer : observers)
			{
				if (observer.get().m_deleted)
				{
					return true;
				}
			}

			return false;
		}
	};

	Delegate() = default;

	virtual ~Delegate() = default;

	template<typename... KArgs>
	void Add(FunctionType &&function, KArgs ...observers)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_functions.emplace_back(FunctionPair{ std::move(function), { std::forward<KArgs>(observers)... } });
	}

	void Remove(const FunctionType &function)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_functions.erase(std::remove_if(m_functions.begin(), m_functions.end(), [function](FunctionPair &f)
		{
			return Hash(f.function) == Hash(function);
		}), m_functions.end());
	}

	void Clear()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_functions.clear();
	}

	typename Invoker::ReturnType operator()(TArgs ... args)
	{
		return Invoker::Invoke(*this, args...);
	}

private:
	friend Invoker;

	static constexpr size_t Hash(const FunctionType &function)
	{
		return function.target_type().hash_code();
	}

	std::mutex m_mutex;
	std::vector<FunctionPair> m_functions;
};
}
