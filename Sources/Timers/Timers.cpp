#include "Timers.hpp"

namespace acid {
Timers::Timers() {
	std::unique_lock<std::mutex> lock(m_mutex);
	m_worker = std::thread(std::bind(&Timers::ThreadRun, this));
}

Timers::~Timers() {
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_stop = true;
	}

	m_condition.notify_all();
	m_worker.join();
}

void Timers::Update() {
}

void Timers::ThreadRun() {
	std::unique_lock<std::mutex> lock(m_mutex);

	while (!m_stop) {
		if (m_timers.empty()) {
			m_condition.wait(lock);
		} else {
			std::sort(m_timers.begin(), m_timers.end(), [](const std::unique_ptr<Timer> &a, const std::unique_ptr<Timer> &b) {
				return a->m_next < b->m_next;
			});

			auto &instance = m_timers.front();
			auto time = Time::Now();

			if (time >= instance->m_next) {
				//Log::Error("Timer % error: ", (time - instance->m_next).AsMilliseconds<float>(), "ms\n");
				lock.unlock();
				instance->m_onTick();
				lock.lock();

				instance->m_next += instance->m_interval;

				if (instance->m_repeat) {
					(*instance->m_repeat)--;

					if (*instance->m_repeat == 0) {
						m_timers.erase(std::remove(m_timers.begin(), m_timers.end(), instance), m_timers.end());
					}
				}
			} else {
				std::chrono::microseconds timePoint(instance->m_next - time);
				m_condition.wait_for(lock, timePoint);
			}
		}
	}
}
}
