#pragma once

#include "../../devices/display.hpp"
#include "../ipostfilter.hpp"

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
		filtertiltshift(const float &blurAmount = 1.0f, const float &centre = 1.1f, const float &stepSize = 0.004f, const float &steps = 3.0f);

		~filtertiltshift();

		void storeValues() override;

		inline void setBlurAmount(const float &blurAmount) { m_blurAmount = blurAmount; }

		inline void setCentre(const float &centre) { m_centre = centre; }

		inline void setStepSize(const float &stepSize) { m_stepSize = stepSize; }

		inline void setSteps(const float &steps) { m_steps = steps; }
	};
}
