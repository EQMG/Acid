#include "IManagerRender.hpp"

#include "Renderer.hpp"

namespace fl
{
	IManagerRender::IManagerRender(std::vector<RenderpassCreate *> renderpassCreate)
	{
		Renderer::Get()->CreateRenderpass(renderpassCreate);
	}

	IManagerRender::~IManagerRender()
	{

	}
}
