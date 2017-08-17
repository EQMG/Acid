#pragma once

#include "../renderer/irenderer.h"
#include "../uis/uis.h"

#include "gui.h"
#include "../models/model.h"

namespace flounder
{
	class rendererguis :
		public irenderer
	{
	private:
		shader *m_shader;

		model *m_model;
	public:
		rendererguis();

		~rendererguis();

		void render(const vector4 &clipPlane, const icamera &camera) override;
	private:
		void prepareRendering(const vector4 &clipPlane, const icamera &camera);

		void renderGui(gui *object);

		void endRendering();
	};
}
