#pragma once

#include <iostream>
#include <functional>

namespace flounder 
{
	/// <summary>
	/// A simple event listener and runner.
	/// </summary>
	class ievent 
	{
	public:
		/// <summary>
		/// Creates a new event.
		/// </summary>
		ievent()
		{
		}

		/// <summary>
		/// Deconstructor for the event.
		/// </summary>
		virtual ~ievent()
		{
		}

		/// <summary>
		/// Gets if the event has occurred.
		/// </summary>
		/// <returns> The event has occurred. </returns>
		virtual bool eventTriggered() = 0;

		/// <summary>
		/// Run when a event has occurred.
		/// </summary>
		virtual void onEvent() = 0;

		/// <summary>
		/// Gets if the event is removed after it has run once.
		/// </summary>
		/// <returns> If the even will run. </returns>
		virtual bool removeAfterEvent() = 0;
	};
}