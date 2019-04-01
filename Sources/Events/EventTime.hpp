#pragma once

#include "Maths/Time.hpp"
#include "Maths/Timer.hpp"
#include "IEvent.hpp"

namespace acid
{
/**
 * Class that runs a event after a time has passed.
 */
class ACID_EXPORT EventTime :
	public IEvent
{
public:
	/**
	 * Creates a new time event.
	 * @param onEvent A function called when the event is triggered.
	 * @param interval The amount of time in the future to run the event.
	 * @param repeat If the event will repeat after the first run.
	 */
	EventTime(std::function<void()> onEvent, const Time &interval, const bool &repeat = false);

	bool EventTriggered() override;

	void OnEvent() override;

	bool RemoveAfterEvent() override { return !m_repeat; }

private:
	std::function<void()> m_onEvent;
	Timer m_timer;
	bool m_repeat;
};
}
