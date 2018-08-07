#include "ThreadPool.hpp"

namespace acid
{
	const uint32_t ThreadPool::HARDWARE_CONCURRENCY = std::thread::hardware_concurrency();

	ThreadPool::ThreadPool(const uint32_t &threadCount)
	{
		for (uint32_t i = 0; i < threadCount; i++)
		{
			m_threads.push_back(std::make_unique<Thread>());
		}
	}

	ThreadPool::~ThreadPool()
	{
	}

	void ThreadPool::Wait()
	{
		for (auto &thread : m_threads)
		{
			thread->Wait();
		}
	}
}
