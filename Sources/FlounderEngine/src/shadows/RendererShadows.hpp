#pragma once

#include "../renderer/IRenderer.hpp"
#include "../terrains/Terrains.hpp"
#include "Shadows.hpp"

namespace Flounder
{
	class RendererShadows :
		public IRenderer
	{
	private:
		static const DescriptorType typeUboObject;
		static const PipelineCreateInfo pipelineCreateInfo;

		UniformBuffer *m_uniformObject;
		Pipeline *m_pipeline;
	public:
		RendererShadows(const int &subpass);

		~RendererShadows();

		void Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	private:
		void RenderModel(const VkCommandBuffer *commandBuffer, Model *object, const Matrix4 &modelMatrix);
	};
}
