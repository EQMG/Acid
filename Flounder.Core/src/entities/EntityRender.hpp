#pragma once

#include "../shaders/Shader.hpp"
#include "../models/Model.hpp"

namespace Flounder
{
	struct EntityRender
	{
		shader *m_shader;
		Model *m_model;
		bool m_shadowRun;
		bool m_undoing;

		EntityRender(shader *shader, Model *model, const bool &shadowRun)
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
