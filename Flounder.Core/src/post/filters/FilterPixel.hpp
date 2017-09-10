#pragma once

#include "../../devices/Display.hpp"
#include "../ipostfilter.hpp"

namespace Flounder
{
	class filterpixel :
		public IPostFilter
	{
	private:
		float m_pixelSize;
	public:
		filterpixel(const float &pixelSize = 2.0f);

		~filterpixel();

		void StoreValues() override;

		void setPixelSize(const float &pixelSize) { m_pixelSize = pixelSize; }
	};
}
