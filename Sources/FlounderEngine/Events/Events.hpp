#pragma once

#include "../Engine/Engine.hpp"
#include "../Tasks/Tasks.hpp"
#include "IEvent.hpp"

namespace Flounder
{
	/// <summary>
	/// A module used for managing events on engine updates.
	/// </summary>
	class Events :
		public IModule
	{
	private:
		std::vector<IEvent *> *m_events;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Events *Get()
		{
			return reinterpret_cast<Events *>(Engine::Get()->GetModule("events"));
		}

		/// <summary>
		/// Creates a new events module.
		/// </summary>
		Events();

		/// <summary>
		/// Deconstructor for the events module.
		/// </summary>
		~Events();

		void Update() override;

		/// <summary>
		/// Adds an event to the listening list.
		/// </summary>
		/// <param name="event"> The event to add. </param>
		void AddEvent(IEvent *event) const;

		/// <summary>
		/// Removes a event to the listening list.
		/// </summary>
		/// <param name="event"> The event to remove. </param>
		void RemoveEvent(IEvent *event) const;
	};
}
