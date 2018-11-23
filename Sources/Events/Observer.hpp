#pragma once

#include <functional>
#include <vector>
#include "Engine/Exports.hpp"

namespace acid
{
	/// <summary>
	/// A class that holds a array of subscribed event callbacks.
	/// </summary>
	template<typename ...Args>
	class Observer
	{
	private:
		std::vector<std::function<void(Args...)>> m_subscribed;
	public:
		/// <summary>
		/// Creates a new callback observer.
		/// </summary>
		Observer() :
			m_subscribed(std::vector<std::function<void(Args...)>>())
		{
		}

		/// <summary>
		/// Run when a event has occurred.
		/// </summary>
		/// <param name="args"> The args to pass to the functions. </param>
		void OnEvent(Args ...args) const
		{
			for (auto &function : m_subscribed)
			{
				function(std::forward<Args>(args)...);
			}
		}

		/// <summary>
		/// Subscribes a function to this observer.
		/// </summary>
		/// <param name="function"> The function to subscribe to this observer. </param>
		void Subscribe(const std::function<void(Args...)> &function)
		{
			m_subscribed.emplace_back(function);
		}

		/// <summary>
		/// Unsubscribes a function to this observer.
		/// </summary>
		/// <param name="function"> The function to unsubscribe to this observer. </param>
		/// <returns> If the function was unsubscribed. </returns>
		bool Unsubscribe(const std::function<void(Args...)> &function)
		{
			for (auto it = --m_subscribed.end(); it != m_subscribed.begin(); --it)
			{
				if ((*it).get() != function)
				{
					continue;
				}

				m_subscribed.erase(it);
				return true;
			}

			return false;
		}
	};
}
