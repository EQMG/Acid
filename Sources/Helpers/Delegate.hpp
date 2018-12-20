#pragma once

#include <list>
#include <memory>
#include <mutex>
#include <vector>
#include <functional>
#include "Engine/Exports.hpp"

namespace acid
{
	template<typename>
	class Delegate;

	template<typename TReturnType, typename... TArgs>
	struct Invoker
	{
	public:
		using ReturnType = std::vector<TReturnType>;

		static ReturnType Invoke(Delegate<TReturnType(TArgs...)> &delegate, TArgs... params)
		{
			std::lock_guard<std::mutex> lock(delegate.m_mutex);
			ReturnType returnValues;

			for (const auto &functionPtr : delegate.m_functionList)
			{
				returnValues.emplace_back((*functionPtr)(params...));
			}

			return returnValues;
		}
	};

	template<typename... TArgs>
	struct Invoker<void, TArgs...>
	{
	public:
		using ReturnType = void;

		static void Invoke(Delegate<void(TArgs...)>& delegate, TArgs... params)
		{
			std::lock_guard<std::mutex> lock(delegate.m_mutex);

			if (delegate.m_functionList.empty())
			{
				return;
			}

			std::for_each(delegate.m_functionList.begin(), delegate.m_functionList.end(), [&](auto &f) {
				f(params...);
			});
		}
	};

	template<typename TReturnType, typename... TArgs>
	class ACID_EXPORT Delegate<TReturnType(TArgs...)>
	{
	private:
		using Invoker = acid::Invoker<TReturnType, TArgs...>;
		using FunctionType = std::function<TReturnType(TArgs...)>;
		friend Invoker;

		std::mutex m_mutex;
		std::vector<FunctionType> m_functionList;
	public:
		Delegate() = default;

		~Delegate() = default;

		Delegate(const Delegate&) = delete;

		const Delegate& operator=(const Delegate&) = delete;

		Delegate &Connect(FunctionType &&function)
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_functionList.emplace_back(function);
			return *this;
		}

		Delegate &Remove(const FunctionType function)
		{
			std::lock_guard<std::mutex> lock(m_mutex);

			m_functionList.remove_if([&](FunctionType &f)
			{
			    return Hash(f) == Hash(function);
			});

			return *this;
		}

		typename Invoker::ReturnType Invoke(TArgs... args)
		{
			return Invoker::Invoke(*this, args...);
		}

		Delegate &Clear()
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_functionList.clear();
			return *this;
		}

		Delegate &operator+=(FunctionType &&function)
		{
			return Connect(std::move(function));
		}

		Delegate &operator-=(const FunctionType function)
		{
			return Remove(function);
		}

		typename Invoker::ReturnType operator()(TArgs... args)
		{
			return Invoker::Invoke(*this, args...);
		}
	private:
		constexpr size_t Hash(const FunctionType &function) const
		{
			return function.target_type().hash_code();
		}
	};
}
