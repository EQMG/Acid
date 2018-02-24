#include "IManagerRender.hpp"

#include "Renderer.hpp"

namespace Flounder
{
	IManagerRender::IManagerRender(RenderpassCreate *renderpassCreate)
	{
		Renderer::Get()->CreateRenderpass(renderpassCreate);
	}

	IManagerRender::~IManagerRender()
	{

	}
}
