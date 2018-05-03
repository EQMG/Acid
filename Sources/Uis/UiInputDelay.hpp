#pragma once

#include "Maths/Timer.hpp"

namespace fl
{
	class FL_EXPORT UiInputDelay
	{
	private:
		Timer *m_timerDelay;
		Timer *m_timerRepeat;
		bool m_delayOver;
	public:
		UiInputDelay();

		~UiInputDelay();

		void Update(const bool &keyIsDown);

		bool CanInput() const;
	};
}
