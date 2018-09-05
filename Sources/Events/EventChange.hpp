#pragma once

#include <functional>
#include "IEvent.hpp"

namespace acid
{
	/// <summary>
	/// A class that acts as a basic change listener for a value.
	/// </summary>
	/// @param <T> The type of value to find change with. </param>
	template<typename T>
	class ACID_EXPORT EventChange :
		public IEvent
	{
	private:
		T *m_reference;
		T m_current;
		std::function<void()> m_onEvent;
		bool m_repeat;
	public:
		/// <summary>
		/// Creates a new change event.
		/// </summary>
		/// <param name="reference"> The reference to listen to. </param>
		/// <param name="onEvent"> A function called when the event is triggered. </param>
		/// <param name="repeat"> If the event will repeat after the first run. </param>
		EventChange(T *reference, const std::function<void()> &onEvent, const bool &repeat = true);

		bool EventTriggered() override;

		void OnEvent() override;

		bool RemoveAfterEvent() override { return !m_repeat; }
	};
}
