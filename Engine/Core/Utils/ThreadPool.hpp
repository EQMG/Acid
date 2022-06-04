#pragma once

#include <thread>
#include <vector>
#include <functional>
#include <mutex>
#include <queue>
#include <future>

#include "CoreExport.hpp"

namespace acid {
/**
 * @brief A fixed size pool of threads.
 */
class ACID_CORE_EXPORT ThreadPool {
public:
	explicit ThreadPool(uint32_t threadCount = std::thread::hardware_concurrency());
	~ThreadPool();

	template<typename F, typename... Args>
	auto Enqueue(F &&f, Args &&... args);

	void Wait();

	const std::vector<std::thread> &GetWorkers() const { return workers; }

private:
	std::vector<std::thread> workers;
	std::queue<std::function<void()>> tasks;

	std::mutex queueMutex;
	std::condition_variable condition;
	bool stop = false;
};

template<typename F, typename ... Args>
auto ThreadPool::Enqueue(F &&f, Args &&... args) {
	using return_type = typename std::invoke_result_t<F, Args ...>;

	auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
	auto result = task->get_future();

	{
		std::unique_lock<std::mutex> lock(queueMutex);

		if (stop)
			throw std::runtime_error("Enqueue called on a stopped ThreadPool");

		tasks.emplace([task]() {
			(*task)();
		});
	}

	condition.notify_one();
	return result;
}
}
