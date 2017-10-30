#include "ComponentAlpha.hpp"

#include "../Entity.hpp"

namespace Flounder
{
	ComponentAlpha::ComponentAlpha(const float &alpha) :
		IComponent(),
		m_alpha(alpha)
	{
	}

	ComponentAlpha::~ComponentAlpha()
	{
	}

	void ComponentAlpha::Update()
	{
	}

	void ComponentAlpha::Render(EntityRender *render)
	{
		if (render->m_shadowRun)
		{
		}
#if 0
		if (!render->m_undoing)
		{
			render->m_shader->loadUniform1f("alpha", m_alpha);
		}
		else
		{
			render->m_shader->loadUniform1f("alpha", 1.0f);
		}
#endif
	}
}
