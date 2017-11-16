#pragma once

#include "../renderer/IRenderer.hpp"
#include "Waters.hpp"

namespace Flounder
{
	class RendererWaters :
		public IRenderer
	{
	private:
		static const DescriptorType typeUboScene;
		static const DescriptorType typeUboObject;
		static const DescriptorType typeSamplerRefraction;
		static const DescriptorType samplerDudv;
		static const PipelineCreateInfo pipelineCreateInfo;

		UniformBuffer *m_uniformScene;
		Pipeline *m_pipeline;
	public:
		RendererWaters(const int &subpass);

		~RendererWaters();

		void Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
