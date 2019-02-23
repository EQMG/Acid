#pragma once

#include <memory>
#include <vector>
#include "Helpers/NonCopyable.hpp"
#include "Thread.hpp"

namespace acid
{
	/// <summary>
	/// A pool of threads.
	/// </summary>
	class ACID_EXPORT ThreadPool :
		public NonCopyable
	{
	public:
		explicit ThreadPool(const uint32_t &threadCount = HardwareConcurrency);

		/// <summary>
		/// Waits until all threads are finished.
		/// </summary>
		void Wait();

		std::vector<std::unique_ptr<Thread>> &GetThreads() { return m_threads; }

		static const uint32_t HardwareConcurrency;
	private:
		std::vector<std::unique_ptr<Thread>> m_threads;
	};
}
