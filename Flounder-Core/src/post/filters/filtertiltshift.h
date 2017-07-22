#pragma once

#include "../../devices/display.h"
#include "../ipostfilter.h"

namespace flounder
{
	class filtertiltshift :
		public ipostfilter
	{
	private:
		float m_blurAmount;
		float m_centre;
		float m_stepSize;
		float m_steps;
	public:
		filtertiltshift(const float &blurAmount, const float &centre, const float &stepSize, const float &steps);

		filtertiltshift();

		~filtertiltshift();

		void storeValues() override;

		inline void setBlurAmount(const float &blurAmount) { m_blurAmount = blurAmount; }

		inline void setCentre(const float &centre) { m_centre = centre; }

		inline void setStepSize(const float &stepSize) { m_stepSize = stepSize; }

		inline void setSteps(const float &steps) { m_steps = steps; }
	};
}
