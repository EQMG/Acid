#include "IManagerRender.hpp"

#include "Renderer.hpp"

namespace fl
{
	IManagerRender::IManagerRender(const std::vector<RenderpassCreate *> &renderpassCreate)
	{
		Renderer::Get()->CreateRenderpass(renderpassCreate);
	}

	IManagerRender::~IManagerRender()
	{

	}
}
