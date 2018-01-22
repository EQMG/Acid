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
		for (auto it = m_events->begin(); it != m_events->end(); ++it)
		{
			delete *it;
		}

		delete m_events;
	}

	void Events::Update()
	{
		for (auto it = m_events->begin(); it < m_events->end(); ++it)
		{
			if ((*it)->EventTriggered())
			{
				(*it)->OnEvent();

				if ((*it)->RemoveAfterEvent())
				{
					m_events->erase(it);
					delete *it;
					return; // it--;
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
