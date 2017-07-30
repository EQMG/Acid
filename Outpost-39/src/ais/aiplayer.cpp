#include "aiplayer.h"

aiplayer::aiplayer(texture *texture, const std::string &name)
{
	m_name = name;
	m_tasks = new std::vector<iaitask*>();
	m_entity = new entity(vector2(0.0f, 0.5f), vector2(0.08f, 0.08f), texture, 1);
	m_entity->addComponent([&](entity *object)
	{
		if (m_tasks->size() > 0)
		{
			iaitask *task = m_tasks->front();
			task->update();

			if (task->isComplete())
			{
				m_tasks->erase(m_tasks->begin());
				delete task;
			}
		}
	});
	// m_entity->setInScreenCoords(false);
	entities::get()->add(m_entity);
}

aiplayer::~aiplayer()
{
	delete m_entity;
	delete m_tasks;
}

void aiplayer::addTask(iaitask *task)
{
	m_tasks->push_back(task);
}

iaitask *aiplayer::getCurrentTask()
{
	return m_tasks->front();
}
