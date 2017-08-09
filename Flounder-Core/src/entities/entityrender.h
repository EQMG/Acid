#pragma once

#include "../shaders/shader.h"
#include "../models/model.h"

namespace flounder
{
	struct entityrender
	{
		shader *m_shader;
		model *m_model;
		bool m_shadowRun;
		bool m_undoing;

		entityrender(shader *shader, model *model, const bool &shadowRun)
		{
			m_shader = shader;
			m_model = model;
			m_shadowRun = shadowRun;
			m_undoing = false;
		}

		~entityrender()
		{
		}
	};
}
