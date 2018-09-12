#pragma once

#include "Maths/Timer.hpp"

namespace acid
{
	class ACID_EXPORT UiInputDelay
	{
	private:
		Timer m_timerDelay;
		Timer m_timerRepeat;
		bool m_delayOver;
	public:
		UiInputDelay();

		void Update(const bool &keyIsDown);

		bool CanInput();
	};
}
