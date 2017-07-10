#pragma once

#include "../framework/framework.h"
#include "ievent.h"

#include "../tasks/tasks.h"

namespace flounder {
	/// <summary>
	/// A module used for managing events on framework updates.
	/// </summary>
	class events : public imodule
	{
	private:
		std::vector<ievent*> *m_events;
	public:
		static events* get()
		{
			return (events*) framework::get()->getInstance("events");
		}

		/// <summary>
		/// Creates a new event manager.
		/// </summary>
		events();

		/// <summary>
		/// Deconstructor for events.
		/// </summary>
		~events();

		void update();

		/// <summary>
		/// Adds an event to the listening que.
		/// </summary>
		/// <param name="event"> The event to add. </param>
		void addEvent(ievent *event);

		/// <summary>
		/// Removes a event to the listening que.
		/// </summary>
		/// <param name="event"> The event to remove. </param>
		void removeEvent(ievent *event);
	};
}
