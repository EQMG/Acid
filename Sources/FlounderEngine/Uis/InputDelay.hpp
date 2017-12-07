#pragma once

#include "../Maths/Timer.hpp"

namespace Flounder
{
	class InputDelay
	{
	private:
		Timer *m_timerDelay;
		Timer *m_timerRepeat;
		bool m_delayOver;
	public:
		InputDelay();

		~InputDelay();

		void Update(const bool &keyIsDown);

		bool CanInput() const;
	};
}
