#pragma once

#include <vector>

namespace flounder 
{
	/// <summary>
	/// Holds requests in a simple que.
	/// </summary>
	template<typename t>
	class queue {
	private:
		std::vector<t> *m_queue;
	public:
		queue()
		{
			m_queue = new std::vector<t>();
		}

		~queue()
		{
			delete m_queue;
		}

		/// <summary>
		/// Adds a new object to queue.
		/// </summary>
		/// <param name="request"> The object to add. </param>
		void addRequest(t request)
		{
			m_queue->push_back(request);
		}

		/// <summary>
		/// Gets the next item in queue and then removes it from this list.
		/// </summary>
		/// <returns> The next item in queue and then removes it from this list. </returns>
		t acceptNextRequest()
		{
			t request = m_queue->front();
			m_queue->erase(m_queue->begin());
			return request;
		}

		/// <summary>
		/// Gets if there are any items left in queue.
		/// </summary>
		/// <returns> Returns true if there are any items left in queue. </returns>
		bool hasRequests()
		{
			return !m_queue->empty();
		}

		/// <summary>
		/// Gets the number of objects in queue.
		/// </summary>
		/// <returns> The number of objects in queue. </returns>
		int count()
		{
			return m_queue->size();
		}

		/// <summary>
		/// Clears the request queue.
		/// </summary>
		void clear()
		{
			m_queue->clear();
		}
	};
}