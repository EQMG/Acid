#pragma once

#include "NonCopyable.hpp"

namespace acid
{
/**
 * @brief A constant-sized non-overwriting circular queue.
 * @tparam T The type to hold.
 */
template<typename T>
class RingBuffer :
	public NonCopyable
{
public:
	explicit RingBuffer(std::size_t capacity) :
		m_data(capacity)
	{
		if (capacity == 0)
		{
			throw std::runtime_error{"Capacity must be non-zero"};
		}
	}

	std::size_t size() const { return m_elements; }

	std::size_t capacity() const { return m_data.capacity(); }

	void resize(std::size_t s)
	{
		if (s == 0)
		{
			throw std::runtime_error{"Capacity must be non-zero"};
		}

		// Don't need to resize.
		if (s == m_data.size())
		{
			return;
		}

		// If size if smaller than number of elements, faux-pop elements.
		if (s < m_elements)
		{
			m_tail += m_elements - s;
			m_tail %= m_data.capacity();
			m_elements = s;
		}

		// Allocate new vector.
		std::vector<T> copy(s);
		std::size_t i = 0;

		// Check for a split buffer.
		if (m_tail > m_head || m_elements > 0)
		{
			// Pop from tail to end.
			while (m_tail < m_data.capacity())
			{
				copy[i++] = m_data[m_tail++];
			}

			m_tail = 0;
		}

		// Pop from tail to head.
		while (m_tail < m_head)
		{
			copy[i++] = m_data[m_tail++];
		}

		// Swap contents.
		m_data.swap(copy);

		// Update new head and tail.
		m_head = m_elements;
		m_tail = 0;
	}

	void clear()
	{
		// Faux-clear.
		m_head = 0;
		m_tail = 0;
		m_elements = 0;
	}

	auto begin()
	{
		return m_data.begin();
	}

	auto begin() const
	{
		return m_data.begin();
	}

	auto end()
	{
		return m_data.end();
	}

	auto end() const
	{
		return m_data.end();
	}

	const T &front() const
	{
		return m_data[m_tail];
	}

	T &front()
	{
		return m_data[m_tail];
	}

	const T &back() const
	{
		std::size_t i = m_head + (m_data.capacity() - 1);
		return m_data[i % m_data.capacity()];
	}

	T &back()
	{
		std::size_t i = m_head + (m_data.capacity() - 1);
		return m_data[i % m_data.capacity()];
	}

	T &operator[](std::size_t i)
	{
		return m_data[i];
	}

	const T &operator[](std::size_t i) const
	{
		return m_data[i];
	}

	const T &at(std::size_t i) const
	{
		std::size_t ind = m_tail + i;
		return m_data[ind % m_data.capacity()];
	}

	T &at(std::size_t i)
	{
		std::size_t ind = m_tail + i;
		return m_data[ind % m_data.capacity()];
	}

	template<typename... Args>
	bool push(Args &&... values)
	{
		std::size_t numElements = NumArgs(values...);

		if (m_elements + numElements > m_data.capacity())
		{
			return false;
		}

		m_elements += numElements;

		// Add data at head.
		MoveAll(m_data, m_head, values...);

		// Move head.
		m_head += numElements;
		m_head %= m_data.capacity();
		return true;
	}

	void pop()
	{
		if (m_elements == 0)
		{
			return;
		}

		m_elements--;

		m_tail++;
		m_tail %= m_data.capacity();
	}

private:
	template<typename K, typename Arg1, typename... Args>
	void MoveAll(std::vector<K> &data, std::size_t i, Arg1 &&v1, Args &&... values)
	{
		data[i % data.size()] = std::forward<Arg1>(v1);
		MoveAll(data, i + 1, values...);
	}

	template<typename K, typename Arg1>
	void MoveAll(std::vector<K> &data, std::size_t i, Arg1 &&v1)
	{
		data[i % data.size()] = std::forward<Arg1>(v1);
	}

	template<typename... K>
	std::size_t NumArgs(K...)
	{
		const std::size_t n = sizeof...(K);
		return n;
	}

	std::size_t NumArgs()
	{
		return 0;
	}

	std::vector<T> m_data;
	std::size_t m_head = 0;
	std::size_t m_tail = 0;
	std::size_t m_elements = 0;
};
}
