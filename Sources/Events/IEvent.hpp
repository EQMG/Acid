#pragma once

#include "Engine/Exports.hpp"

namespace acid
{
	/// <summary>
	/// A simple event listener and runner.
	/// </summary>
	class ACID_EXPORT IEvent
	{
	public:
		/// <summary>
		/// Gets if the event has occurred.
		/// </summary>
		/// <returns> The event has occurred. </returns>
		virtual bool EventTriggered() = 0;

		/// <summary>
		/// Run when a event has occurred.
		/// </summary>
		virtual void OnEvent() = 0;

		/// <summary>
		/// Gets if the event is removed after it has run once.
		/// </summary>
		/// <returns> If the even will run. </returns>
		virtual bool RemoveAfterEvent() = 0;
	};
}
