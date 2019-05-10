#include "Timers.hpp"

namespace acid
{
Timers::Timers() :
	m_stop(false)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_worker = std::thread(std::bind(&Timers::Run, this));
}

Timers::~Timers()
{
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_stop = true;
	}

	m_condition.notify_all();
	m_worker.join();
}

void Timers::Update()
{
}

void Timers::Run()
{
	std::unique_lock<std::mutex> lock(m_mutex);

	while (true)
	{
		if (m_timers.empty())
		{
			m_condition.wait(lock);
		}
		else
		{
			/*m_timers.erase(std::remove_if(m_timers.begin(), m_timers.end(), [](const std::unique_ptr<TimerInstance> &x)
			{
				return x->m_destroyed;
			}), m_timers.end());*/
			std::sort(m_timers.begin(), m_timers.end(), [](const std::unique_ptr<TimerInstance> &a, const std::unique_ptr<TimerInstance> &b)
			{
				return a->m_next < b->m_next;
			});

			auto it = m_timers.begin();
			auto &instance = *it;
			auto time = Engine::GetTime();

			if (time >= instance->m_next)
			{
				lock.unlock();
				instance->m_onTick();
				lock.lock();

				instance->m_next += instance->m_interval;

				if (instance->m_repeat)
				{
					(*instance->m_repeat)--;

					if (*instance->m_repeat == 0)
					{
						m_timers.erase(it);
					}
				}
			}
			else
			{
				auto timePoint = std::chrono::time_point<std::chrono::steady_clock>(std::chrono::microseconds(instance->m_next));
				m_condition.wait_until(lock, timePoint);
			}
		}
	}
}
}
