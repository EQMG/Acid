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

		inline void setScreenColour(const colour &screenColour) { m_screenColour->set(screenColour); }

		inline void setCurveAmountX(const float &curveAmountX) { m_curveAmountX = curveAmountX; }

		inline void setCurveAmountY(const float &curveAmountY) { m_curveAmountY = curveAmountY; }

		inline void setScanLineSize(const float &scanLineSize) { m_scanLineSize = scanLineSize; }

		inline void setScanIntensity(const float &scanIntensity) { m_scanIntensity = scanIntensity; }
	};
}
