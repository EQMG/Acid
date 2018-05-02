#pragma once

#include "Engine/Exports.hpp"

namespace fl
{
	/// <summary>
	/// A class for handing and calculating deltas.
	/// </summary>
	class FL_EXPORT Delta
	{
	private:
		float m_currentFrameTime;
		float m_lastFrameTime;
		float m_change;
		float m_time;
	public:
		/// <summary>
		/// Creates a new change handler.
		/// </summary>
		Delta();

		/// <summary>
		/// Deconstructor for the delta.
		/// </summary>
		~Delta();

		/// <summary>
		/// Updates change and times.
		/// </summary>
		void Update();

		float GetChange() const { return m_change; }

		float GetTime() const { return m_time; }
	};
}
