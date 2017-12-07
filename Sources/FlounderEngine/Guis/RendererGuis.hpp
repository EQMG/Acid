#pragma once

#include "../Renderer/IRenderer.hpp"
#include "../Renderer/pipelines/Pipeline.hpp"

namespace Flounder
{
	class RendererGuis :
		public IRenderer
	{
	private:
		static const DescriptorType typeUboObject;
		static const DescriptorType typeSamplerTexture;
		static const PipelineCreateInfo pipelineCreateInfo;

		Pipeline *m_pipeline;
	public:
		RendererGuis(const int &subpass);

		~RendererGuis();

		void Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
