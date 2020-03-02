#pragma once

#include <stdexcept>
#include <vector>

#include "NonCopyable.hpp"

namespace acid {
/**
 * @brief A constant-sized non-overwriting circular queue.
 * @tparam T The type to hold.
 */
template<typename T>
class RingBuffer : NonCopyable {
public:
	explicit RingBuffer(std::size_t capacity) :
		data(capacity) {
		if (capacity == 0)
			throw std::runtime_error("Capacity must be non-zero");
	}

	std::size_t size() const { return elements; }
	std::size_t capacity() const { return data.capacity(); }

	void resize(std::size_t s) {
		if (s == 0)
			throw std::runtime_error("Capacity must be non-zero");

		// Don't need to resize.
		if (s == data.size()) {
			return;
		}

		// If size if smaller than number of elements, faux-pop elements.
		if (s < elements) {
			tail += elements - s;
			tail %= data.capacity();
			elements = s;
		}

		// Allocate new vector.
		std::vector<T> copy(s);
		std::size_t i = 0;

		// Check for a split buffer.
		if (tail > head || elements > 0) {
			// Pop from tail to end.
			while (tail < data.capacity()) {
				copy[i++] = data[tail++];
			}

			tail = 0;
		}

		// Pop from tail to head.
		while (tail < head) {
			copy[i++] = data[tail++];
		}

		// Swap contents.
		data.swap(copy);

		// Update new head and tail.
		head = elements;
		tail = 0;
	}

	void clear() {
		// Faux-clear.
		head = 0;
		tail = 0;
		elements = 0;
	}

	auto begin() { return data.begin(); }
	auto begin() const { return data.begin(); }

	auto end() { return data.end(); }
	auto end() const { return data.end(); }

	const T &front() const {
		return data[tail];
	}

	T &front() {
		return data[tail];
	}

	const T &back() const {
		std::size_t i = head + (data.capacity() - 1);
		return data[i % data.capacity()];
	}

	T &back() {
		std::size_t i = head + (data.capacity() - 1);
		return data[i % data.capacity()];
	}

	T &operator[](std::size_t i) { return data[i]; }
	const T &operator[](std::size_t i) const { return data[i]; }

	const T &at(std::size_t i) const {
		std::size_t ind = tail + i;
		return data[ind % data.capacity()];
	}

	T &at(std::size_t i) {
		std::size_t ind = tail + i;
		return data[ind % data.capacity()];
	}

	template<typename... Args>
	bool push(Args &&... values) {
		auto numElements = NumArgs(values...);

		if (elements + numElements > data.capacity()) {
			return false;
		}

		elements += numElements;

		// Add data at head.
		MoveAll(data, head, values...);

		// Move head.
		head += numElements;
		head %= data.capacity();
		return true;
	}

	void pop() {
		if (elements == 0) {
			return;
		}

		elements--;

		tail++;
		tail %= data.capacity();
	}

private:
	template<typename K, typename Arg1, typename... Args>
	void MoveAll(std::vector<K> &data, std::size_t i, Arg1 &&v1, Args &&... values) {
		data[i % data.size()] = std::forward<Arg1>(v1);
		MoveAll(data, i + 1, values...);
	}

	template<typename K, typename Arg1>
	void MoveAll(std::vector<K> &data, std::size_t i, Arg1 &&v1) {
		data[i % data.size()] = std::forward<Arg1>(v1);
	}

	template<typename... K>
	std::size_t NumArgs(K ...) {
		const std::size_t n = sizeof...(K);
		return n;
	}

	std::size_t NumArgs() {
		return 0;
	}

	std::vector<T> data;
	std::size_t head = 0;
	std::size_t tail = 0;
	std::size_t elements = 0;
};
}
