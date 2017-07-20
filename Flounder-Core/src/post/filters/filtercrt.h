#pragma once

#include "../../maths/colour.h"
#include "../ipostfilter.h"

namespace flounder 
{
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
		filtercrt(colour *screenColour, const float &curveAmountX, const float &curveAmountY, const float &scanLineSize, const float &scanIntensity);

		~filtercrt();

		void storeValues() override;
	};
}
