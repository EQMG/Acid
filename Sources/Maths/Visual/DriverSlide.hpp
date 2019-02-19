#pragma once

#include "IDriver.hpp"

namespace acid
{
	/// <summary>
	/// A driver that slides to its destination using cosine interpolation.
	/// </summary>
	template<typename T>
	class DriverSlide :
		public IDriver<T>
	{
	public:
		/// <summary>
		/// Creates a new slide driver.
		/// </summary>
		/// <param name="start"> The start value. </param>
		/// <param name="end"> The end value. </param>
		/// <param name="length"> The time to get to the end value. </param>
		DriverSlide(const T &start, const T &end, const Time &length) :
			IDriver<T>(length),
			m_start(start),
			m_end(end)
		{
		}

		/// <summary>
		/// Gets the start time.
		/// </summary>
		/// <returns> The start time. </returns>
		const T &GetStart() const { return m_start; }

		/// <summary>
		/// Sets the start time.
		/// </summary>
		/// <param name="start"> The new start time. </param>
		void SetStart(const T &start) { m_start = start; }

		/// <summary>
		/// Gets the end time.
		/// </summary>
		/// <returns> The end time. </returns>
		const T &GetEnd() const { return m_end; }

		/// <summary>
		/// Sets the end time.
		/// </summary>
		/// <param name="end"> The new end time. </param>
		void SetEnd(const T &end) { m_end = end; }
	protected:
		T Calculate(const float &factor) override
		{
			float realTime = std::min(IDriver<T>::m_actualTime, IDriver<T>::GetLength()) / IDriver<T>::GetLength();
			return m_start + realTime * (m_end - m_start);
			//	return Maths::CosInterpolate(m_start, m_end, realTime);
		}

	private:
		T m_start;
		T m_end;
	};
}
