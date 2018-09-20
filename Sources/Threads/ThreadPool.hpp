#pragma once

#include <memory>
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

		explicit ThreadPool(const uint32_t &threadCount = HARDWARE_CONCURRENCY);

		ThreadPool(const ThreadPool&) = delete; 

		ThreadPool& operator=(const ThreadPool&) = delete;

		/// <summary>
		/// Waits until all threads are finished.
		/// </summary>
		void Wait();

		std::vector<std::unique_ptr<Thread>> &GetThreads() { return m_threads; }
	};
}
