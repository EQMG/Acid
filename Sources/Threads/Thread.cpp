#include "Thread.hpp"

namespace acid
{
Thread::Thread() : m_worker(std::thread(&Thread::QueueLoop, this)) {}

Thread::~Thread()
{
	if(m_worker.joinable())
		{
			Wait();
			m_queueMutex.lock();
			m_destroying = true;
			m_condition.notify_one();
			m_queueMutex.unlock();
			m_worker.join();
		}
}

void Thread::AddJob(std::function<void()>& job)
{
	std::lock_guard<std::mutex> lock(m_queueMutex);
	m_jobQueue.push(std::move(job));
	m_condition.notify_one();
}

void Thread::Wait()
{
	std::unique_lock<std::mutex> lock(m_queueMutex);
	m_condition.wait(lock, [&]() { return m_jobQueue.empty(); });
}

void Thread::QueueLoop()
{
	while(true)
		{
			std::function<void()> job;

			{
				std::unique_lock<std::mutex> lock(m_queueMutex);
				m_condition.wait(lock, [&] { return !m_jobQueue.empty() || m_destroying; });

				if(m_destroying)
					{
						break;
					}

				job = m_jobQueue.front();
			}

			job();

			{
				std::lock_guard<std::mutex> lock(m_queueMutex);
				m_jobQueue.pop();
				m_condition.notify_one();
			}
		}
}
}
