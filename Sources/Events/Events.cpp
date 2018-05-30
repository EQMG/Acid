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
		for (auto event : m_events)
		{
			delete event;
		}
	}

	void Events::Update()
	{
		std::vector<IEvent *> copy = std::vector<IEvent *>(m_events); // TODO: Remove copying.

		for (auto event : copy)
		{
			if (event->EventTriggered())
			{
				event->OnEvent();

				if (event->RemoveAfterEvent())
				{
					RemoveEvent(event);
				}
			}
		}
	}

	void Events::AddEvent(IEvent *event)
	{
		m_events.push_back(event);
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
