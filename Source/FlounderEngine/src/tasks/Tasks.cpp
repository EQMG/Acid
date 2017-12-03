#include "Tasks.hpp"

namespace Flounder
{
	Tasks::Tasks() :
		IModule(),
		m_tasks(new std::vector<std::function<void()>>())
	{
	}

	Tasks::~Tasks()
	{
		delete m_tasks;
	}

	void Tasks::Update()
	{
		for (auto it = m_tasks->begin(); it != m_tasks->end(); ++it)
		{
			(*it)();
		}

		m_tasks->clear();
	}

	void Tasks::AddTask(std::function<void()> task) const
	{
		m_tasks->push_back(task);
	}
}
