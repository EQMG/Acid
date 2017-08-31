#pragma once

#include "../../camera/camera.hpp"
#include "../../devices/Display.hpp"
#include "../../maths/vector3.hpp"
#include "../ipostfilter.hpp"

namespace Flounder
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

		void setSunHeight(const float &sunHeight) { m_sunHeight = sunHeight; }
	};
}
