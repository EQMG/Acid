#include "tasks.h"

namespace flounder
{
	tasks::tasks() :
		imodule()
	{
		m_tasks = new std::vector<std::function<void()>>();
	}

	tasks::~tasks()
	{
		delete m_tasks;
	}

	void tasks::update()
	{
		for (std::vector<std::function<void()>>::iterator it = m_tasks->begin(); it != m_tasks->end(); ++it)
		{
			(*it)();
		}

		m_tasks->clear();
	}

	void tasks::addTask(std::function<void()> task)
	{
		m_tasks->push_back(task);
	}
}
