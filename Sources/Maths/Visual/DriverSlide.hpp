#pragma once

#include "Driver.hpp"

namespace acid {
/**
 * @brief A driver that slides to its destination using cosine interpolation.
 * @tparam T The type to be driven.
 **/
template<typename T>
class DriverSlide : public Driver<T> {
public:
	/**
	 * Creates a new slide driver.
	 * @param start The start value.
	 * @param end The end value.
	 * @param length The time to get to the end value.
	 **/
	DriverSlide(const T &start, const T &end, const Time &length) :
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
	 * @return The end time.
	 **/
	const T &GetEnd() const { return m_end; }

	/**
	 * Sets the end time.
	 * @param end The new end time.
	 **/
	void SetEnd(const T &end) { m_end = end; }

protected:
	T Calculate(float factor) override {
		auto realTime = static_cast<float>(std::min(Driver<T>::m_actualTime, Driver<T>::GetLength()) / Driver<T>::GetLength());
		return m_start + realTime * (m_end - m_start);
		//return Maths::CosInterpolate(m_start, m_end, realTime);
	}

private:
	T m_start;
	T m_end;
};
}
