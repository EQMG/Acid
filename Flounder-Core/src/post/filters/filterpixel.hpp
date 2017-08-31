#pragma once

#include "../../devices/display.hpp"
#include "../ipostfilter.hpp"

namespace Flounder
{
	class filterpixel :
		public ipostfilter
	{
	private:
		float m_pixelSize;
	public:
		filterpixel(const float &pixelSize = 2.0f);

		~filterpixel();

		void storeValues() override;

		inline void setPixelSize(const float &pixelSize) { m_pixelSize = pixelSize; }
	};
}
