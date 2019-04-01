#pragma once

#include "Engine/Engine.hpp"
#include "IEvent.hpp"

namespace acid
{
/**
 * @brief Module used for managing events on engine updates.
 */
class ACID_EXPORT Events :
	public Module
{
public:
	/**
	 * Gets the engines instance.
	 * @return The current module instance.
	 */
	static Events *Get() { return Engine::Get()->GetModuleManager().Get<Events>(); }

	Events();

	void Update() override;

	/**
	 * Adds an event to the listening list.
	 * @param event The event to add.
	 * @return The added event.
	 */
	IEvent *AddEvent(IEvent *event);

	/**
	 * Adds an event to the listening list.
	 * @tparam T The type of event to add.
	 * @tparam Args The args types.
	 * @param args The type event arguments.
	 */
	template<typename T, typename... Args>
	void AddEvent(Args &&... args) { AddEvent(new T(std::forward<Args>(args)...)); }

	/**
	 * Removes a event to the listening list.
	 * @param event The event to remove.
	 */
	void RemoveEvent(IEvent *event);

private:
	std::vector<std::unique_ptr<IEvent>> m_events;
};
}
