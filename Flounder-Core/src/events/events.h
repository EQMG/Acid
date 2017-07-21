#pragma once

#include "../framework/framework.h"
#include "../tasks/tasks.h"

#include "ievent.h"

namespace flounder
{
	/// <summary>
	/// A module used for managing events on framework updates.
	/// </summary>
	class events : public imodule
	{
	private:
		std::vector<ievent*> *m_events;
	public:
		/// <summary>
		/// Gets this framework instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static events *get()
		{
			return static_cast<events*>(framework::get()->getInstance("events"));
		}

		/// <summary>
		/// Creates a new events module.
		/// </summary>
		events();

		/// <summary>
		/// Deconstructor for the events module.
		/// </summary>
		~events();

		void init() override;

		void update() override;

		/// <summary>
		/// Adds an event to the listening list.
		/// </summary>
		/// <param name="event"> The event to add. </param>
		void addEvent(ievent *event);

		/// <summary>
		/// Removes a event to the listening list.
		/// </summary>
		/// <param name="event"> The event to remove. </param>
		void removeEvent(ievent *event);
	};
}
