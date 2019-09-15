#include "ThreadPool.hpp"

namespace acid {
ThreadPool::ThreadPool(uint32_t threadCount) {
	m_workers.reserve(threadCount);

	for (std::size_t i = 0; i < threadCount; ++i) {
		m_workers.emplace_back([this] {
			while (true) {
				std::function<void()> task;

				{
					std::unique_lock<std::mutex> lock(m_queueMutex);
					m_condition.wait(lock, [this] {
						return m_stop || !m_tasks.empty();
					});

					if (m_stop && m_tasks.empty())
						return;

					task = std::move(m_tasks.front());
					m_tasks.pop();
				}

				task();
			}
		});
	}
}

ThreadPool::~ThreadPool() {
	{
		std::unique_lock<std::mutex> lock(m_queueMutex);
		m_stop = true;
	}

	m_condition.notify_all();

	for (auto &worker : m_workers)
		worker.join();
}

void ThreadPool::Wait() {
	std::unique_lock<std::mutex> lock(m_queueMutex);

	m_condition.wait(lock, [this]() {
		return m_tasks.empty();
	});
}
}
