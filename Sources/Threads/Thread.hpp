#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include "Engine/Exports.hpp"

namespace acid
{
	class ACID_EXPORT Thread
	{
	private:
		std::thread m_worker;
		std::queue<std::function<void()>> m_jobQueue;
		std::mutex m_queueMutex;
		std::condition_variable m_condition;
		bool m_destroying = false;
	public:
		Thread();

		~Thread();

		/// <summary>
		/// Adds an job to the thread queue.
		/// </summary>
		/// <param name="job"> The job to add. </param>
		void AddJob(std::function<void()> job);

		/// <summary>
		/// Waits until all jobs have been finished.
		/// </summary>
		void Wait();
	private:
		void QueueLoop();
	};
}
