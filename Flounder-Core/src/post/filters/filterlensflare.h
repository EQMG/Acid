#pragma once

#include "../../camera/camera.h"
#include "../../devices/display.h"
#include "../../maths/vector3.h"
#include "../ipostfilter.h"

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
