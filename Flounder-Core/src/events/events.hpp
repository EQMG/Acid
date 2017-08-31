#pragma once

#include "../engine/Engine.hpp"
#include "../tasks/tasks.hpp"

#include "ievent.hpp"

namespace Flounder
{
	/// <summary>
	/// A module used for managing events on engine updates.
	/// </summary>
	class events : public IModule
	{
	private:
		std::vector<ievent*> *m_events;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static events *get()
		{
			return static_cast<events*>(Engine::Get()->GetModule("events"));
		}

		/// <summary>
		/// Creates a new events module.
		/// </summary>
		events();

		/// <summary>
		/// Deconstructor for the events module.
		/// </summary>
		~events();

		void Update() override;

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
