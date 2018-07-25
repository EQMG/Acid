#include "Tasks.hpp"

namespace acid
{
	Tasks::Tasks() :
		IModule(),
		m_tasks(std::vector<std::function<void()>>())
	{
	}

	Tasks::~Tasks()
	{
	}

	void Tasks::Update()
	{
		for (auto it = m_tasks.begin(); it != m_tasks.end(); ++it)
		{
			(*it)();
		}

		m_tasks.clear();
	}

	void Tasks::AddTask(std::function<void()> task)
	{
		m_tasks.emplace_back(task);
	}
}
