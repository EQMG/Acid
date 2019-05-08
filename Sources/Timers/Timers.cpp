#include "Timers.hpp"

namespace acid
{
Timers::Timers()
{
}

Timers::~Timers()
{
}

void Timers::Update()
{
	for (auto it = m_timers.begin(); it != m_timers.end();)
	{
		if ((*it)->m_destroyed || (*it)->IsFinished())
		{
			it = m_timers.erase(it);
			continue;
		}

		if ((*it)->m_start + (*it)->m_intervel <= Engine::Get()->GetTime())
		{
			auto error = Engine::Get()->GetTime() - ((*it)->m_start + (*it)->m_intervel);
			Log::Out("Timer Error: %f\n", error.AsSeconds());

			(*it)->m_start = Engine::Get()->GetTime();

			if ((*it)->m_repeat)
			{
				(*(*it)->m_repeat)--;
			}

			(*it)->m_onTick();
		}

		it++;
	}
}
}
