#pragma once

#include "../models/Model.hpp"

namespace Flounder
{
	struct EntityRender
	{
		Model *m_model;
		bool m_shadowRun;
		bool m_undoing;

		EntityRender(Model *model, const bool &shadowRun)
		{
			m_model = model;
			m_shadowRun = shadowRun;
			m_undoing = false;
		}

		~EntityRender()
		{
		}
	};
}
