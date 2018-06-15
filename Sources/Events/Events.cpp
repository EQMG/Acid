#include "Events.hpp"

namespace fl
{
	Events::Events() :
		IModule(),
		m_events(std::vector<IEvent *>())
	{
	}

	Events::~Events()
	{
		for (auto &event : m_events)
		{
			delete event;
		}
	}

	void Events::Update()
	{
		for (auto &event : std::vector<IEvent *>(m_events)) // TODO: Remove copying.
		{
			if (!event->EventTriggered())
			{
				continue;
			}

			event->OnEvent();

			if (event->RemoveAfterEvent())
			{
				RemoveEvent(event);
			}
		}
	}

	void Events::AddEvent(IEvent *event)
	{
		m_events.emplace_back(event);
	}

	void Events::RemoveEvent(IEvent *event)
	{
		for (auto it = m_events.begin(); it != m_events.end(); ++it)
		{
			if (*it == event)
			{
				m_events.erase(it);
				return;
			}
		}
	}
}
