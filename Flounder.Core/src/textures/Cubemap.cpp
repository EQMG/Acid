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
