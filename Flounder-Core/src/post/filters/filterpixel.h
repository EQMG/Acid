#pragma once

#include "../../devices/display.h"
#include "../ipostfilter.h"

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
