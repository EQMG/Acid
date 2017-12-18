#pragma once

#include "../Renderer/IRenderer.hpp"
#include "../Renderer/Buffers/UniformBuffer.hpp"
#include "../Renderer/Pipelines/Pipeline.hpp"

namespace Flounder
{
	class RendererTerrains :
		public IRenderer
	{
	private:
		static const DescriptorType typeUboScene;
		static const DescriptorType typeUboObject;
		static const PipelineCreateInfo pipelineCreateInfo;

		UniformBuffer *m_uniformScene;
		Pipeline *m_pipeline;
	public:
		RendererTerrains(const int &subpass);

		~RendererTerrains();

		void Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
