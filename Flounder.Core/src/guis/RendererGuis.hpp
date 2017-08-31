#pragma once

#include "../renderer/irenderer.hpp"
#include "../uis/uis.hpp"

#include "gui.hpp"
#include "../models/model.hpp"

namespace Flounder
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

		void render(const Vector4 &clipPlane, const ICamera &camera) override;
	private:
		void prepareRendering(const Vector4 &clipPlane, const ICamera &camera);

		void renderGui(gui *object);

		void endRendering();
	};
}
