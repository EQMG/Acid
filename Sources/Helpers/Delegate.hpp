#pragma once

#include <algorithm>
#include <functional>
#include <mutex>
#include <vector>

namespace acid
{
template<typename>
class Delegate;

template<typename TReturnType, typename... TArgs>
struct Invoker
{
	using ReturnType = std::vector<TReturnType>;

	static ReturnType Invoke(Delegate<TReturnType(TArgs...)>& delegate, TArgs... params)
	{
		std::lock_guard<std::mutex> lock(delegate.m_mutex);
		ReturnType returnValues;

		for(const auto& functionPtr : delegate.m_functionList)
			{
				returnValues.emplace_back((*functionPtr)(params...));
			}

		return returnValues;
	}
};

template<typename... TArgs>
struct Invoker<void, TArgs...>
{
	using ReturnType = void;

	static void Invoke(Delegate<void(TArgs...)>& delegate, TArgs... params)
	{
		std::lock_guard<std::mutex> lock(delegate.m_mutex);

		if(delegate.m_functionList.empty())
			{
				return;
			}

		std::for_each(delegate.m_functionList.begin(), delegate.m_functionList.end(), [&](auto& f) { f(params...); });
	}
};

template<typename TReturnType, typename... TArgs>
class Delegate<TReturnType(TArgs...)> final
{
  public:
	using Invoker = acid::Invoker<TReturnType, TArgs...>;
	using FunctionType = std::function<TReturnType(TArgs...)>;

	Delegate() = default;

	virtual ~Delegate() = default;

	Delegate& Connect(FunctionType&& function)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_functionList.emplace_back(function);
		return *this;
	}

	Delegate& Remove(const FunctionType function)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		m_functionList.remove_if([&](FunctionType& f) { return Hash(f) == Hash(function); });

		return *this;
	}

	typename Invoker::ReturnType Invoke(TArgs... args)
	{
		return Invoker::Invoke(*this, args...);
	}

	Delegate& Clear()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_functionList.clear();
		return *this;
	}

	Delegate& operator+=(FunctionType&& function)
	{
		return Connect(std::move(function));
	}

	Delegate& operator-=(const FunctionType function)
	{
		return Remove(function);
	}

	typename Invoker::ReturnType operator()(TArgs... args)
	{
		return Invoker::Invoke(*this, args...);
	}

  private:
	friend Invoker;

	static constexpr size_t Hash(const FunctionType& function)
	{
		return function.target_type().hash_code();
	}

	std::mutex m_mutex;
	std::vector<FunctionType> m_functionList;
};
}
