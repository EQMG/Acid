#pragma once

#include "../../devices/Display.hpp"
#include "../ipostfilter.hpp"

namespace Flounder
{
	class filterblurhorizontal :
		public ipostfilter
	{
	private:
		int m_widthValue;
		float m_scaleValue;
		bool m_fitToDisplay;
		float m_sizeScalar;
	public:
		filterblurhorizontal(const float &sizeScalar);

		filterblurhorizontal(const int &width, const int &height);

		~filterblurhorizontal();

		void storeValues() override;

		void setScaleValue(const float &scaleValue) { m_scaleValue = scaleValue; }
	};
}
