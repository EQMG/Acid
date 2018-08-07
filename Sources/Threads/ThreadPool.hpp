#pragma once

#include <vector>
#include "Engine/Exports.hpp"
#include "Thread.hpp"

namespace acid
{
	/// <summary>
	/// A pool of threads.
	/// </summary>
	class ACID_EXPORT ThreadPool
	{
	private:
		std::vector<std::unique_ptr<Thread>> m_threads;
	public:
		static const uint32_t HARDWARE_CONCURRENCY;

		ThreadPool(const uint32_t &threadCount = HARDWARE_CONCURRENCY);

		~ThreadPool();

		/// <summary>
		/// Waits until all threads are finished.
		/// </summary>
		void Wait();

		std::vector<std::unique_ptr<Thread>> &GetThreads() { return m_threads; }
	};
}
