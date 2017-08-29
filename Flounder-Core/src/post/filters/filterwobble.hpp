#pragma once

#include "../../devices/display.hpp"
#include "../../framework/framework.hpp"
#include "../ipostfilter.hpp"

namespace flounder
{
	class filterwobble :
		public ipostfilter
	{
	private:
		float m_wobbleSpeed;
		float m_wobbleAmount;
	public:
		filterwobble(const float &wobbleSpeed);

		filterwobble();

		~filterwobble();

		void storeValues() override;

		inline void setWobbleSpeed(const float &wobbleSpeed) { m_wobbleSpeed = wobbleSpeed; }
	};
}
