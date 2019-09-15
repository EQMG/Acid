#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include "StdAfx.hpp"

namespace acid {
/**
 * @brief A fixed size pool of threads.
 */
class ACID_EXPORT ThreadPool {
public:
	explicit ThreadPool(uint32_t threadCount = std::thread::hardware_concurrency());
	~ThreadPool();

	template<typename F, typename... Args>
	decltype(auto) Enqueue(F &&f, Args &&... args);

	void Wait();

	const std::vector<std::thread> &GetWorkers() const { return m_workers; }

private:
	std::vector<std::thread> m_workers;
	std::queue<std::function<void()>> m_tasks;

	std::mutex m_queueMutex;
	std::condition_variable m_condition;
	bool m_stop = false;
};

template<typename F, typename ... Args>
decltype(auto) ThreadPool::Enqueue(F &&f, Args &&... args) {
	using return_type = typename std::result_of<F(Args...)>::type;

	auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
	auto result = task->get_future();

	{
		std::unique_lock<std::mutex> lock(m_queueMutex);

		if (m_stop)
			throw std::runtime_error("Enqueue called on a stopped ThreadPool");

		m_tasks.emplace([task]() {
			(*task)();
		});
	}

	m_condition.notify_one();
	return result;
}
}
