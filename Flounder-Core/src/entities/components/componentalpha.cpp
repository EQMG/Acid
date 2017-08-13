#include "componentalpha.h"

namespace flounder
{
	componentalpha::componentalpha(const float &alpha) :
		icomponent()
	{
		m_alpha = alpha;
	}

	componentalpha::~componentalpha()
	{
	}

	void componentalpha::update()
	{
	}

	void componentalpha::render(entityrender *render)
	{
		if (render->m_shadowRun)
		{
			return;
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
