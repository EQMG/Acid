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

	//Time startWait;
	//Time lengthWait;
	//bool setWait = false;

	while (!m_stop)
	{
		if (m_timers.empty())
		{
			m_condition.wait(lock);
		}
		else
		{
			//if (setWait)
			//{
			//	Log::Debug("End Wait: %fms, error: %fms\n", Time::Now().AsMilliseconds<float>(), (Time::Now() - (startWait + lengthWait)).AsMilliseconds<float>());
			//	setWait = false;
			//}

			std::sort(m_timers.begin(), m_timers.end(), [](const std::unique_ptr<TimerInstance> &a, const std::unique_ptr<TimerInstance> &b)
			{
				return a->m_next < b->m_next;
			});

			auto &instance = m_timers.front();
			auto time = Time::Now();

			if (time >= instance->m_next)
			{
				//Log::Warning("Timer error: %fms\n", (time - instance->m_next).AsMilliseconds<float>());
				lock.unlock();
				instance->m_onTick();
				lock.lock();

				instance->m_next += instance->m_interval;

				if (instance->m_repeat)
				{
					(*instance->m_repeat)--;

					if (*instance->m_repeat == 0)
					{
						m_timers.erase(std::remove(m_timers.begin(), m_timers.end(), instance), m_timers.end());
					}
				}
			}
			else
			{
				auto timePoint = std::chrono::microseconds(instance->m_next - time);
				//Log::Debug("Start Wait: %fms, waiting: %fms\n", time.AsMilliseconds<float>(), (instance->m_next - time).AsMilliseconds<float>());
				//startWait = time;
				//lengthWait = instance->m_next - time;
				//setWait = true;
				m_condition.wait_for(lock, timePoint);
			}
		}
	}
}
}
