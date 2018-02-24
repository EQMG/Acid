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
		UniformBuffer *m_uniformObject;
		Pipeline *m_pipeline;
	public:
		RendererShadows(const int &subpass);

		~RendererShadows();

		void Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;

	private:
		void RenderModel(const VkCommandBuffer &commandBuffer, Model *object, GameObject *gameObject);
	};
}
