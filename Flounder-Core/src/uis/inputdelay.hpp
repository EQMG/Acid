#pragma once

#include "../maths/timer.hpp"

namespace flounder
{
	class inputdelay
	{
	private:
		timer *m_timerDelay;
		timer *m_timerRepeat;
		bool m_delayOver;
	public:
		inputdelay();

		~inputdelay();

		void update(const bool &keyIsDown);

		bool canInput();
	};
}
