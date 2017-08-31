#pragma once

#include "../shaders/Shader.hpp"
#include "../models/Model.hpp"

namespace Flounder
{
	struct EntityRender
	{
		shader *m_shader;
		model *m_model;
		bool m_shadowRun;
		bool m_undoing;

		EntityRender(shader *shader, model *model, const bool &shadowRun)
		{
			m_shader = shader;
			m_model = model;
			m_shadowRun = shadowRun;
			m_undoing = false;
		}

		~EntityRender()
		{
		}
	};
}
