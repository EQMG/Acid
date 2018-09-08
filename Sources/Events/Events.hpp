#pragma once

#include <vector>
#include "Engine/Engine.hpp"
#include "IEvent.hpp"

namespace acid
{
	/// <summary>
	/// A module used for managing events on engine updates.
	/// </summary>
	class ACID_EXPORT Events :
		public IModule
	{
	private:
		std::vector<std::unique_ptr<IEvent>> m_events;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Events *Get() { return Engine::Get()->GetModule<Events>(); }

		Events();

		~Events();

		Events(const Events&) = delete; // FIXME: Temp Fix.

		Events& operator=(const Events&) = delete;

		void Update() override;

		/// <summary>
		/// Adds an event to the listening list.
		/// </summary>
		/// <param name="event"> The event to add. </param>
		/// <returns> The added event. </returns>
		IEvent *AddEvent(IEvent *event);

		/// <summary>
		/// Adds an event to the listening list.
		/// </summary>
		/// <param name="T"> The type of event to add. </param>
		/// <param name="args"> The type event arguments. </param>
		template<typename T, typename... Args>
		void AddEvent(Args &&... args) { AddEvent(new T(std::forward<Args>(args)...)); }

		/// <summary>
		/// Removes a event to the listening list.
		/// </summary>
		/// <param name="event"> The event to remove. </param>
		/// <returns> If the event was removed. </returns>
		bool RemoveEvent(IEvent *event);
	};
}
