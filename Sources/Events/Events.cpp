#include "Events.hpp"

namespace acid
{
	Events::Events() :
		m_events(std::vector<std::unique_ptr<IEvent>>())
	{
	}

	Events::~Events()
	{
	}

	void Events::Update()
	{
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
		m_events.emplace_back(event);
		return event;
	}

	bool Events::RemoveEvent(IEvent *event)
	{
		for (auto it = m_events.begin(); it != m_events.end(); ++it)
		{
			if ((*it).get() == event)
			{
				m_events.erase(it);
				return true;
			}
		}

		return false;
	}
}
