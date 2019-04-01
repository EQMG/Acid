#pragma once

#include "IEvent.hpp"

namespace acid
{
/**
 * Class that is a basic implementation of the event interface.
 */
class ACID_EXPORT EventStandard :
	public IEvent
{
public:
	/**
	 * Creates a new standard event.
	 * @param onEvent A function called when the event is triggered.
	 * @param triggered A function called to check if the event was triggered.
	 * @param repeat If the event will repeat after the first run.
	 */
	EventStandard(std::function<void()> onEvent, std::function<bool()> triggered, const bool &repeat = false);

	bool EventTriggered() override;

	void OnEvent() override;

	bool RemoveAfterEvent() override { return !m_repeat; }

private:
	std::function<void()> m_onEvent;
	std::function<bool()> m_triggered;
	bool m_repeat;
};
}
