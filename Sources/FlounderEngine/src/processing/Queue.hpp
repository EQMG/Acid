#pragma once

#include <vector>

namespace Flounder
{
	/// <summary>
	/// Holds requests in a simple que.
	/// </summary>
	template<typename T>
	class Queue
	{
	private:
		std::vector<T> *m_queue;
	public:
		/// <summary>
		/// Creates a new queue.
		/// </summary>
		Queue()
		{
			m_queue = new std::vector<T>();
		}

		/// <summary>
		/// Deconstructor for the queue.
		/// </summary>
		~Queue()
		{
			delete m_queue;
		}

		/// <summary>
		/// Adds a new object to queue.
		/// </summary>
		/// <param name="request"> The object to add. </param>
		void AddRequest(T request)
		{
			m_queue->push_back(request);
		}

		/// <summary>
		/// Gets the next item in queue and then removes it from this list.
		/// </summary>
		/// <returns> The next item in queue and then removes it from this list. </returns>
		T AcceptNextRequest()
		{
			T request = m_queue->front();
			m_queue->erase(m_queue->begin());
			return request;
		}

		/// <summary>
		/// Gets if there are any items left in queue.
		/// </summary>
		/// <returns> Returns true if there are any items left in queue. </returns>
		bool HasRequests()
		{
			return !m_queue->empty();
		}

		/// <summary>
		/// Gets the number of objects in queue.
		/// </summary>
		/// <returns> The number of objects in queue. </returns>
		int Count()
		{
			return m_queue->size();
		}

		/// <summary>
		/// Clears the request queue.
		/// </summary>
		void Clear()
		{
			m_queue->Clear();
		}
	};
}
