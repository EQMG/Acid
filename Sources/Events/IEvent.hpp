#pragma once

#include "StdAfx.hpp"

namespace acid
{
/**
 * @brief A simple event listener and runner.
 */
class ACID_EXPORT IEvent
{
public:
	IEvent() = default;

	virtual ~IEvent() = default;

	/**
	 * Gets if the event has occurred.
	 * @return The event has occurred.
	 */
	virtual bool EventTriggered() = 0;

	/**
	 * Run when a event has occurred.
	 */
	virtual void OnEvent() = 0;

	/**
	 * Gets if the event is removed after it has run once.
	 * @return If the even will run.
	 */
	virtual bool RemoveAfterEvent() = 0;
};
}
