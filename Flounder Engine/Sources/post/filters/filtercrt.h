#pragma once

#include "../../maths/colour.h"
#include "../ipostfilter.h"

namespace flounder {
	class filtercrt :
		public ipostfilter
	{
	private:
		colour *m_screenColour;
		float m_curveAmountX;
		float m_curveAmountY;
		float m_scanLineSize;
		float m_scanIntensity;
	public:
		filtercrt(colour *screenColour, float curveAmountX, float curveAmountY, float scanLineSize, float scanIntensity);

		~filtercrt();

		void storeValues() override;
	};
}
