#pragma once

#include "../../maths/Colour.hpp"
#include "../IPostFilter.hpp"

namespace Flounder
{
	class FilterCrt :
		public IPostFilter
	{
	private:
		Colour *m_screenColour;
		float m_curveAmountX;
		float m_curveAmountY;
		float m_scanLineSize;
		float m_scanIntensity;
	public:
		FilterCrt(const Colour &screenColour = Colour(0.5f, 1.0f, 0.5f), const float &curveAmountX = 0.1f, const float &curveAmountY = 0.1f, const float &scanLineSize = 1000.0f, const float &scanIntensity = 0.1f);

		~FilterCrt();

		void StoreValues() override;

		Colour *GetScreenColour() const { return m_screenColour; }

		void SetScreenColour(const Colour &screenColour) const { m_screenColour->Set(screenColour); }

		float GetCurveAmountX() const { return m_curveAmountX; }

		void SetCurveAmountX(const float &curveAmountX) { m_curveAmountX = curveAmountX; }

		float GetCurveAmountY() const { return m_curveAmountY; }

		void SetCurveAmountY(const float &curveAmountY) { m_curveAmountY = curveAmountY; }

		float GetScanLineSize() const { return m_scanLineSize; }

		void SetScanLineSize(const float &scanLineSize) { m_scanLineSize = scanLineSize; }

		float GetScanIntensity() const { return m_scanIntensity; }

		void SetScanIntensity(const float &scanIntensity) { m_scanIntensity = scanIntensity; }
	};
}
