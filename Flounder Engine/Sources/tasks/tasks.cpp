#include "tasks.h"

namespace flounder {
	tasks::tasks()
	{
		m_tasks = new std::vector<std::function<void()>>();
	}

	tasks::~tasks()
	{
		delete m_tasks;
	}

	void tasks::update()
	{
		for (auto task : *m_tasks) 
		{
			task();
		}

		m_tasks->clear();
	}

	void tasks::addTask(std::function<void()> task)
	{
		m_tasks->push_back(task);
	}
}
