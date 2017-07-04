#pragma once

namespace flounder {
	/// <summary>
	/// A class for handing and calculating deltas.
	/// </summary>
	class delta
	{
	private:
		double m_currentFrameTime = 0;
		double m_lastFrameTime = 0;

	public:
		double m_change = 0;
		double m_time = 0;

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
		void update();
	};
}
