#pragma once

#include <mutex>
#include <list>
#include <vector>
#include <memory>
#include <functional>
#include "Engine/Exports.hpp"

namespace acid
{
	template<typename>
	class Delegate;

	namespace DelegateImpl
	{
		template <typename TReturnType, typename... TArgs>
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
					returnValues.push_back((*functionPtr)(params...));
				}

				return returnValues;
			}
		};

		template <typename... TArgs>
		struct Invoker<void, TArgs...>
		{
		public:
			using ReturnType = void;

			static void Invoke(Delegate<void(TArgs...)> &delegate, TArgs... params)
			{
				std::lock_guard<std::mutex> lock(delegate.m_mutex);

				for (const auto &functionPtr : delegate.m_functionList)
				{
					(*functionPtr)(params...);
				}
			}
		};
	}

	template<typename TReturnType, typename... TArgs>
	class ACID_EXPORT Delegate<TReturnType(TArgs...)>
	{
	private:
		using Invoker = DelegateImpl::Invoker<TReturnType, TArgs...>;
		using functionType = std::function<TReturnType(TArgs...)>;

		friend Invoker;

		std::mutex m_mutex;
		std::list<std::shared_ptr<functionType>> m_functionList;
	public:
		Delegate() = default;

		~Delegate() = default;

		Delegate(const Delegate&) = delete;

		const Delegate& operator =(const Delegate&) = delete;

		Delegate& Connect(const functionType &function)
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_functionList.push_back(std::make_shared<functionType>(function));
			return *this;
		}

		Delegate& Remove(const functionType &function)
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_functionList.remove_if([&](std::shared_ptr<functionType> &functionPtr)
			                         {
				                         return Hash(function) == Hash(*functionPtr);
			                         });
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

		Delegate& operator+=(const functionType &function)
		{
			return Connect(function);
		}

		Delegate& operator-=(const functionType &function)
		{
			return Remove(function);
		}

		typename Invoker::ReturnType operator()(TArgs... args)
		{
			return Invoker::Invoke(*this, args...);
		}
	private:
		constexpr size_t Hash(const functionType &function) const
		{
			return function.target_type().hash_code();
		}
	};
}
