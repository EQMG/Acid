#include "IManagerRender.hpp"

#include "Renderer.hpp"

namespace Flounder
{
	IManagerRender::IManagerRender(std::vector<RenderpassCreate *> renderpassCreate)
	{
		Renderer::Get()->CreateRenderpass(renderpassCreate);
	}

	IManagerRender::~IManagerRender()
	{

	}
}
