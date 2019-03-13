#include "Events.hpp"

namespace acid
{
Events::Events() {}

void Events::Update()
{
	for(auto it = m_events.begin(); it != m_events.end();)
		{
			if(!(*it)->EventTriggered())
				{
					++it;
					continue;
				}

			(*it)->OnEvent();

			if((*it)->RemoveAfterEvent())
				{
					it = m_events.erase(it);
					continue;
				}

			++it;
		}
}

IEvent* Events::AddEvent(IEvent* event)
{
	m_events.emplace_back(event);
	return event;
}

void Events::RemoveEvent(IEvent* event)
{
	m_events.erase(std::remove_if(m_events.begin(), m_events.end(), [&](std::unique_ptr<IEvent>& e) { return e.get() == event; }), m_events.end());
}
}
