#pragma once

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include "StdAfx.hpp"

namespace acid
{
	/// <summary>
	/// A pool of threads.
	/// </summary>
	class ACID_EXPORT ThreadPool
	{
	public:
		explicit ThreadPool(const uint32_t &threadCount = HardwareConcurrency);

		~ThreadPool();

		template<class F, class... Args>
		auto Enqueue(F &&f, Args &&... args) -> 
			std::future<typename std::result_of<F(Args...)>::type>
		{
			using return_type = typename std::result_of<F(Args...)>::type;

			auto task = std::make_shared<std::packaged_task<return_type()>>(
				std::bind(std::forward<F>(f), std::forward<Args>(args)...));

			std::future<return_type> result = task->get_future();

			{
				std::unique_lock<std::mutex> lock(m_queueMutex);

				// Don't allow enqueueing after stopping the pool.
				if (m_stop)
				{
					throw std::runtime_error("Enqueue called on on stopped ThreadPool");
				}

				m_tasks.emplace([task]() { (*task)(); });
			}

			m_condition.notify_one();
			return result;
		}

		static const uint32_t HardwareConcurrency;
	private:
		std::vector<std::thread> m_workers;
		std::queue<std::function<void()>> m_tasks;

		std::mutex m_queueMutex;
		std::condition_variable m_condition;
		bool m_stop;
	};
}
