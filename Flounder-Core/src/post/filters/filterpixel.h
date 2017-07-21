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

		inline float getPixelSize() { return m_pixelSize; }

		inline void setPixelSize(const float &pixelSize) { m_pixelSize = pixelSize; }
	};
}
