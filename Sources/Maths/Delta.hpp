#pragma once

#include "Engine/Exports.hpp"
#include "Time.hpp"

namespace acid
{
	/// <summary>
	/// A class for handing and calculating deltas.
	/// </summary>
	class ACID_EXPORT Delta
	{
	private:
		Time m_currentFrameTime;
		Time m_lastFrameTime;
		Time m_change;
		Time m_time;
	public:
		/// <summary>
		/// Creates a new change handler.
		/// </summary>
		Delta();

		/// <summary>
		/// Updates change and times.
		/// </summary>
		void Update();

		Time GetChange() const { return m_change; }

		Time GetTime() const { return m_time; }
	};
}
