#pragma once

#include "Engine/Engine.hpp"
#include "Tasks/Tasks.hpp"
#include "IEvent.hpp"

namespace fl
{
	/// <summary>
	/// A module used for managing events on engine updates.
	/// </summary>
	class FL_EXPORT Events :
		public IModule
	{
	private:
		std::vector<std::shared_ptr<IEvent>> m_events;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Events *Get()
		{
			return Engine::Get()->GetModule<Events>();
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
		/// <returns> The added event. </returns>
		std::shared_ptr<IEvent> AddEvent(std::shared_ptr<IEvent> event);

		/// <summary>
		/// Adds an event to the listening list.
		/// </summary>
		/// <param name="T"> The type of event to add. </param>
		/// <param name="args"> The type event arguments. </param>
		template<typename T, typename... Args>
		void AddEvent(Args &&... args) { AddEvent(std::make_shared<T>(std::forward<Args>(args)...)); }

		/// <summary>
		/// Removes a event to the listening list.
		/// </summary>
		/// <param name="event"> The event to remove. </param>
		/// <returns> The removed event. </returns>
		std::shared_ptr<IEvent> RemoveEvent(std::shared_ptr<IEvent> event);
	};
}
