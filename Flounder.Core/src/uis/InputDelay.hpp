#pragma once

#include "../maths/timer.hpp"

namespace Flounder
{
	class inputdelay
	{
	private:
		Timer *m_timerDelay;
		Timer *m_timerRepeat;
		bool m_delayOver;
	public:
		inputdelay();

		~inputdelay();

		void update(const bool &keyIsDown);

		bool canInput();
	};
}
