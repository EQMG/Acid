#pragma once

namespace flounder {
	/// <summary>
	/// A class for handing and calculating deltas.
	/// </summary>
	class delta
	{
	private:
		double m_currentFrameTime;
		double m_lastFrameTime;

		double m_change;
		double m_time;
	public:
		/// <summary>
		/// Creates a new change handler.
		/// </summary>
		delta();

		/// <summary>
		/// Deconstructor for the delta.
		/// </summary>
		~delta();

		/// <summary>
		/// Updates change and times.
		/// </summary>
		/// <param name="timeMs"> The current time in ms. </param>
		void update(float timeMs);

		/// <summary>
		/// Gets the current change.
		/// </summary>
		/// <returns> The change. </returns>
		double getChange();

		/// <summary>
		/// Gets the time, all deltas added up.
		/// </summary>
		/// <returns> The time. </returns>
		double getTime();
	};
}
