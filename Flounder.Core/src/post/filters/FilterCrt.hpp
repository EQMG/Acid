#pragma once

#include "../../maths/colour.hpp"
#include "../ipostfilter.hpp"

namespace Flounder
{
	class filtercrt :
		public ipostfilter
	{
	private:
		Colour *m_screenColour;
		float m_curveAmountX;
		float m_curveAmountY;
		float m_scanLineSize;
		float m_scanIntensity;
	public:
		filtercrt(const Colour &screenColour = Colour(0.5f, 1.0f, 0.5f, 1.0f), const float &curveAmountX = 0.1f, const float &curveAmountY = 0.1f, const float &scanLineSize = 1000.0f, const float &scanIntensity = 0.1f);

		~filtercrt();

		void storeValues() override;

		void setScreenColour(const Colour &screenColour) { m_screenColour->Set(screenColour); }

		void setCurveAmountX(const float &curveAmountX) { m_curveAmountX = curveAmountX; }

		void setCurveAmountY(const float &curveAmountY) { m_curveAmountY = curveAmountY; }

		void setScanLineSize(const float &scanLineSize) { m_scanLineSize = scanLineSize; }

		void setScanIntensity(const float &scanIntensity) { m_scanIntensity = scanIntensity; }
	};
}
