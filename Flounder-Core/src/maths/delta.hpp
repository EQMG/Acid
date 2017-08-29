#pragma once

#include "../framework/framework.hpp"

#include "maths.hpp"

namespace flounder
{
	/// <summary>
	/// A class for handing and calculating deltas.
	/// </summary>
	class delta
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
		delta();

		/// <summary>
		/// Deconstructor for the delta.
		/// </summary>
		~delta();

		/// <summary>
		/// Updates change and times.
		/// </summary>
		void update();

		float getChange() const { return m_change; }

		float getTime() const { return m_time; }
	};
}
