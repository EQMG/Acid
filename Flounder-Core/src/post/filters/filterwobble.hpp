#pragma once

#include "../../devices/display.hpp"
#include "../../engine/Engine.hpp"
#include "../ipostfilter.hpp"

namespace Flounder
{
	class filterwobble :
		public ipostfilter
	{
	private:
		float m_wobbleSpeed;
		float m_wobbleAmount;
	public:
		filterwobble(const float &wobbleSpeed = 2.0f);

		~filterwobble();

		void storeValues() override;

		inline void setWobbleSpeed(const float &wobbleSpeed) { m_wobbleSpeed = wobbleSpeed; }
	};
}
