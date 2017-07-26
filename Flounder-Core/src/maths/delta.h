#pragma once

#include "../framework/framework.h"

#include "maths.h"

namespace flounder
{
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
		void update();

		double getChange() const { return m_change; }

		double getTime() const { return m_time; }
	};
}
