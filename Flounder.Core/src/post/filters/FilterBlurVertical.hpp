#pragma once

#include "../../devices/Display.hpp"
#include "../ipostfilter.hpp"

namespace Flounder
{
	class filterblurvertical :
		public IPostFilter
	{
	private:
		int m_heightValue;
		float m_scaleValue;
		bool m_fitToDisplay;
		float m_sizeScalar;
	public:
		filterblurvertical(const float &sizeScalar);

		filterblurvertical(const int &width, const int &height);

		~filterblurvertical();

		void StoreValues() override;

		void setScaleValue(const float &scaleValue) { m_scaleValue = scaleValue; }
	};
}
