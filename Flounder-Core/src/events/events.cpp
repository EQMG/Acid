#include "events.hpp"

namespace Flounder
{
	events::events() :
		IModule(),
		m_events(new std::vector<ievent*>())
	{
	}

	events::~events()
	{
		for (auto it = m_events->begin(); it != m_events->end(); ++it)
		{
			delete *it;
		}

		delete m_events;
	}

	void events::Update()
	{
		for (auto it = m_events->begin(); it < m_events->end(); it++)
		{
			if ((*it)->eventTriggered())
			{
				(*it)->onEvent();

				if ((*it)->removeAfterEvent())
				{
					m_events->erase(it);
					delete *it;
					return; // it--;
				}
			}
		}
	}

	void events::addEvent(ievent *event)
	{
		m_events->push_back(event);
	}

	void events::removeEvent(ievent *event)
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
