#pragma once

#include "Texture.hpp"

namespace Flounder
{
	class Cubemap
	{
	private:
		Texture *m_front;
		Texture *m_back;
		Texture *m_left;
		Texture *m_right;
		Texture *m_top;
		Texture *m_bottom;

		uint32_t m_width, m_height;
		uint32_t m_layerCount = 1;
		VkDeviceSize m_imageSize;
	public:
		Cubemap(const std::string &filename, const std::string &fileExt);

		~Cubemap();
	};
}
