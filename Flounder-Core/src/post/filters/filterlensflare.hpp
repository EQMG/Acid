#pragma once

#include "../../camera/camera.hpp"
#include "../../devices/display.hpp"
#include "../../maths/vector3.hpp"
#include "../ipostfilter.hpp"

namespace flounder
{
	class filterlensflare :
		public ipostfilter
	{
	private:
		vector3 *m_sunPosition;
		float m_sunHeight;
	public:
		filterlensflare();

		~filterlensflare();

		void storeValues() override;

		void setSunPosition(const vector3 &sunPosition);

		inline void setSunHeight(const float &sunHeight) { m_sunHeight = sunHeight; }
	};
}
