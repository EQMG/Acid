#pragma once

#include <flounder.h>

#include "iaitask.h"

using namespace flounder;

class aiplayer
{
private:
	std::string m_name;
	std::vector<iaitask*> *m_tasks;
	entity *m_entity;
public:
	aiplayer(texture *texture, const std::string &name);

	~aiplayer();

	void addTask(iaitask* task);

	iaitask *getCurrentTask();

	template<typename t>
	bool containsTask();

	std::string getName() const { return m_name; }

	entity *getEntity() const { return m_entity; }
};

template<typename t>
inline bool aiplayer::containsTask()
{
	for (iaitask *task : *m_tasks)
	{
		if (static_cast<t*>(task) != NULL)
		{
			return true;
		}
	}

	return false;
}
