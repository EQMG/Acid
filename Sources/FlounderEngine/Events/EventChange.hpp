#pragma once

#include <functional>
#include "IEvent.hpp"

namespace fl
{
	/// <summary>
	/// A class that acts as a basic change listener for a value.
	/// </summary>
	/// @param <T> The type of value to find change with. </param>
	template<typename T>
	class FL_EXPORT EventChange :
		public IEvent
	{
	private:
		T *m_reference;
		T m_current;
		std::function<void()> m_onEvent;
	public:
		/// <summary>
		/// Creates a new change event.
		/// </summary>
		/// <param name="reference"> The reference to listen to. </param>
		/// <param name="onEvent"> A function called when the event is triggered. </param>
		EventChange(T *reference, const std::function<void()> &onEvent);

		bool EventTriggered() override;

		void OnEvent() override;

		bool RemoveAfterEvent() override { return false; }
	};
}
