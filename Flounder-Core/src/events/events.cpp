#include "events.h"

namespace flounder 
{
	events::events()
		: imodule()
	{
		m_events = new std::vector<ievent*>();
	}

	events::~events()
	{
		for (std::vector<ievent*>::iterator it = m_events->begin(); it != m_events->end(); ++it)
		{
			delete *it;
		}

		delete m_events;
	}

	void events::init()
	{
	}

	void events::update()
	{
		for (std::vector<ievent*>::iterator it = m_events->begin(); it < m_events->end(); it++)
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
		for (std::vector<ievent*>::iterator it = m_events->begin(); it != m_events->end(); ++it)
		{
			if (*it == event)
			{
				m_events->erase(it);
				return;
			}
		}
	}
}
