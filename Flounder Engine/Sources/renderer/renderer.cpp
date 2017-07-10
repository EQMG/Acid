#include "renderer.h"

namespace flounder {
	renderer::renderer()
		: imodule()
	{
		m_renderermaster = NULL;
	}

	renderer::~renderer()
	{
		delete m_renderermaster;
	}

	void renderer::loadRendererMaster(irenderermaster *renderermaster)
	{
		m_renderermaster = renderermaster;
	}

	void renderer::init()
	{
	}

	void renderer::update()
	{
		if (m_renderermaster != NULL)
		{
			m_renderermaster->render();
		}
	}
}
