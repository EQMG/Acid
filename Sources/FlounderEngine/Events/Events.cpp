#include "Events.hpp"

namespace Flounder
{
	Events::Events() :
		IModule(),
		m_events(new std::vector<IEvent *>())
	{
	}

	Events::~Events()
	{
		for (auto event : *m_events)
		{
			delete event;
		}

		delete m_events;
	}

	void Events::Update()
	{
		std::vector<IEvent *> copy = std::vector<IEvent *>(*m_events);

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

	void Events::AddEvent(IEvent *event) const
	{
		m_events->push_back(event);
	}

	void Events::RemoveEvent(IEvent *event) const
	{
		for (auto it = m_events->begin(); it != m_events->end(); ++it)
		{
			if (*it == event)
			{
				m_events->erase(it);
				return;
			}
		}
	}
}
