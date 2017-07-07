#include "tasks.h"

namespace flounder {
	tasks::tasks()
	{
		m_tasks = std::vector<itask*>();
	}

	tasks::~tasks()
	{
	}

	void tasks::update()
	{
		for (auto *task : m_tasks) 
		{
			task->execute();
		}

		m_tasks.clear();
	}

	inline void tasks::addTask(itask *task)
	{
		m_tasks.push_back(task);
	}

	inline void tasks::removeTask(itask *task)
	{
		std::vector<itask*>::iterator it;

		for (it = m_tasks.begin(); it != m_tasks.end(); ++it)
		{
			if (*it == task)
			{
				it = m_tasks.erase(it);
				--it;
				return;
			}
		}
	}
}
