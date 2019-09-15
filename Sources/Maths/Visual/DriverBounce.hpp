#pragma once

#include "Maths/Maths.hpp"
#include "Driver.hpp"

namespace acid {
/**
 * @brief A bounce driver that uses a sine wave.
 * @tparam T The type to be driven.
 **/
template<typename T>
class DriverBounce : public Driver<T> {
public:
	/**
	 * Creates a new sine wave driver.
	 * @param start The start value.
	 * @param end The end value.
	 * @param length The length between two waves.
	 **/
	DriverBounce(const T &start, const T &end, const Time &length) :
		Driver<T>(length),
		m_start(start),
		m_end(end) {
	}

	/**
	 * Gets the start time.
	 * @return The start time.
	 **/
	const T &GetStart() const { return m_start; }

	/**
	 * Sets the start time.
	 * @param start The new start time.
	 **/
	void SetStart(const T &start) { m_start = start; }

	/**
	 * Gets the end time.
	 * @return The ebd time.
	 **/
	const T &GetEnd() const { return m_end; }

	/**
	 * Sets the end time.
	 * @param end The new end time.
	 **/
	void SetEnd(const T &end) { m_end = end; }

protected:
	T Calculate(float factor) override {
		auto value = 0.5f + std::sin(Maths::Pi<float> * 2.0f * factor) * 0.5f;

		if (Driver<T>::m_actualTime > Driver<T>::GetLength() / 2.0f) {
			value = 0.0f;
		}

		return m_start + value * (m_end - m_start);
	}

private:
	T m_start;
	T m_end;
};
}
