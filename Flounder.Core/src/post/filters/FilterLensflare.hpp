#pragma once

#include "../../camera/camera.hpp"
#include "../../devices/Display.hpp"
#include "../../maths/Vector3.hpp"
#include "../ipostfilter.hpp"

namespace Flounder
{
	class filterlensflare :
		public IPostFilter
	{
	private:
		Vector3 *m_sunPosition;
		float m_sunHeight;
	public:
		filterlensflare();

		~filterlensflare();

		void StoreValues() override;

		void setSunPosition(const Vector3 &sunPosition);

		void setSunHeight(const float &sunHeight) { m_sunHeight = sunHeight; }
	};
}
