#pragma once

#include "../renderer/IRenderer.hpp"
#include "../uis/Uis.hpp"

#include "Gui.hpp"
#include "../models/Model.hpp"

namespace Flounder
{
	class RendererGuis :
		public IRenderer
	{
	private:
		Shader *m_shader;

		Model *m_model;
	public:
		RendererGuis();

		~RendererGuis();

		void Render(const Vector4 &clipPlane, const ICamera &camera) override;
	private:
		void PrepareRendering(const Vector4 &clipPlane, const ICamera &camera);

		void RenderGui(Gui *object);

		void EndRendering();
	};
}
