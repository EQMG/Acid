#include "FilterPixel.hpp"

#include "../../devices/Display.hpp"

namespace Flounder
{
	FilterPixel::FilterPixel(const int &subpass) :
		IPostFilter("filterGrain", "res/shaders/filters/grain.frag.spv", subpass, {}),
		m_pixelSize(2.0f)
	{
	}

	FilterPixel::~FilterPixel()
	{
	}

	void FilterPixel::RenderFilter(const VkCommandBuffer *commandBuffer)
	{
#if 0
		m_shader->loadUniform1f("pixelSize", m_pixelSize);
		m_shader->loadUniform2f("displaySize", static_cast<float>(Display::Get()->getWidth()), static_cast<float>(Display::Get()->getHeight()));
#endif
	}
}
