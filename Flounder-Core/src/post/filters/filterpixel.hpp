#pragma once

#include "../../devices/display.hpp"
#include "../ipostfilter.hpp"

namespace flounder
{
	class filterpixel :
		public ipostfilter
	{
	private:
		float m_pixelSize;
	public:
		filterpixel(const float &pixelSize);

		filterpixel();

		~filterpixel();

		void storeValues() override;

		inline void setPixelSize(const float &pixelSize) { m_pixelSize = pixelSize; }
	};
}
