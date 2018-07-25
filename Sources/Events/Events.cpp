#include "Events.hpp"

namespace acid
{
	Events::Events() :
		IModule(),
		m_events(std::vector<std::shared_ptr<IEvent>>())
	{
	}

	Events::~Events()
	{
	}

	void Events::Update()
	{
		for (auto &event : std::vector<std::shared_ptr<IEvent>>(m_events)) // TODO: Remove copying.
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

	std::shared_ptr<IEvent> Events::AddEvent(std::shared_ptr<IEvent> event)
	{
		m_events.emplace_back(event);
		return event;
	}

	std::shared_ptr<IEvent> Events::RemoveEvent(std::shared_ptr<IEvent> event)
	{
		for (auto it = m_events.begin(); it != m_events.end(); ++it)
		{
			if (*it == event)
			{
				m_events.erase(it);
				return *it;
			}
		}

		return nullptr;
	}
}
