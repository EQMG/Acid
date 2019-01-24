#include "Events.hpp"

#include <algorithm>

namespace acid
{
	Events::Events() :
		m_mutex(std::mutex()),
		m_events(std::vector<std::unique_ptr<IEvent>>())
	{
	}

	void Events::Update()
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		for (auto it = m_events.begin(); it != m_events.end();)
		{
			if (!(*it)->EventTriggered())
			{
				++it;
				continue;
			}

			(*it)->OnEvent();

			if ((*it)->RemoveAfterEvent())
			{
				it = m_events.erase(it);
				continue;
			}

			++it;
		}
	}

	IEvent *Events::AddEvent(IEvent *event)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		m_events.emplace_back(event);
		return event;
	}

	void Events::RemoveEvent(IEvent *event)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		m_events.erase(std::remove_if(m_events.begin(), m_events.end(), [&](std::unique_ptr<IEvent> &e) {
			return e.get() == event;
		}), m_events.end());
	}
}
