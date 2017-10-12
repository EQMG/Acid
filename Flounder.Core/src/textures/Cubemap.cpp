#include "Cubemap.hpp"

namespace Flounder
{
	Cubemap::Cubemap(const std::string &filename, const std::string &fileExt) :
		m_front(new Texture(filename + "Front" + fileExt)),
		m_back(new Texture(filename + "Back" + fileExt)),
		m_left(new Texture(filename + "Left" + fileExt)),
		m_right(new Texture(filename + "Right" + fileExt)),
		m_top(new Texture(filename + "Top" + fileExt)),
		m_bottom(new Texture(filename + "Bottom" + fileExt)),

		m_width(1024),
		m_height(1024),
		m_layerCount(1),
		m_imageSize((1024 * 1024 * 4) * 6)
	{
		stbi_uc* pix = (stbi_uc*)malloc(m_imageSize);
		stbi_uc* offset = pix;
		int width, height, components;

		stbi_uc *pixelsFront = Texture::LoadPixels(filename + "Front" + fileExt, &width, &height, &components);
		offset += m_front->GetSize();
		std::memcpy(offset, pixelsFront, m_front->GetSize());

		stbi_uc *pixelsBack = Texture::LoadPixels(filename + "Back" + fileExt, &width, &height, &components);
		offset += m_front->GetSize();
		std::memcpy(offset, pixelsBack, m_front->GetSize());

		stbi_uc *pixelsLeft = Texture::LoadPixels(filename + "Left" + fileExt, &width, &height, &components);
		offset += m_front->GetSize();
		std::memcpy(offset, pixelsLeft, m_front->GetSize());

		stbi_uc *pixelsRight = Texture::LoadPixels(filename + "Right" + fileExt, &width, &height, &components);
		offset += m_front->GetSize();
		std::memcpy(offset, pixelsRight, m_front->GetSize());

		stbi_uc *pixelsTop = Texture::LoadPixels(filename + "Top" + fileExt, &width, &height, &components);
		offset += m_front->GetSize();
		std::memcpy(offset, pixelsTop, m_front->GetSize());

		stbi_uc *pixelsBottom = Texture::LoadPixels(filename + "Bottom" + fileExt, &width, &height, &components);
		offset += m_front->GetSize();
		std::memcpy(offset, pixelsBottom, m_front->GetSize());

		delete pixelsFront;
		delete pixelsBack;
		delete pixelsLeft;
		delete pixelsRight;
		delete pixelsTop;
		delete pixelsBottom;
	}

	Cubemap::~Cubemap()
	{
		delete m_front;
		delete m_back;
		delete m_left;
		delete m_right;
		delete m_top;
		delete m_bottom;
	}
}
