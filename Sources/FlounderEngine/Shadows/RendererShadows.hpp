#pragma once

#include "../Renderer/IRenderer.hpp"
#include "../Terrains/Terrains.hpp"
#include "../Models/Model.hpp"
#include "Shadows.hpp"

namespace Flounder
{
	class F_EXPORT RendererShadows :
		public IRenderer
	{
	private:
		UniformBuffer *m_uniformScene;
		Pipeline *m_pipeline;
	public:
		RendererShadows(const GraphicsStage &graphicsStage);

		~RendererShadows();

		void Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
