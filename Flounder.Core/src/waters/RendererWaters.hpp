#pragma once

#include "../renderer/IRenderer.hpp"
#include "Waters.hpp"

namespace Flounder
{
	class RendererWaters :
		public IRenderer
	{
	private:
		static const DescriptorType RendererWaters::typeUboScene;
		static const DescriptorType RendererWaters::typeUboObject;
		//	static const DescriptorType RendererWaters::samplerReflections;
		static const PipelineCreateInfo RendererWaters::pipelineCreateInfo;

		UniformBuffer *m_uniformScene;
		Pipeline *m_pipeline;
	public:
		RendererWaters();

		~RendererWaters();

		void Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
