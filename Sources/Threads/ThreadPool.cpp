#include "ThreadPool.hpp"

namespace acid
{
const uint32_t ThreadPool::HardwareConcurrency = std::thread::hardware_concurrency();

ThreadPool::ThreadPool(const uint32_t& threadCount)
{
	for(uint32_t i = 0; i < threadCount; i++)
		{
			m_threads.emplace_back(std::make_unique<Thread>());
		}
}

void ThreadPool::Wait()
{
	for(auto& thread : m_threads)
		{
			thread->Wait();
		}
}
}
