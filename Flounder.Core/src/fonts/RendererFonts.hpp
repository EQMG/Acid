#pragma once

#include "../renderer/IRenderer.hpp"
#include "../renderer/pipelines/Pipeline.hpp"
#include "Text.hpp"

namespace Flounder
{
	class RendererFonts :
		public IRenderer
	{
	private:
		static const DescriptorType typeUboObject;
		static const DescriptorType typeSamplerTexture;
		static const PipelineCreateInfo pipelineCreateInfo;

		Pipeline *m_pipeline;
	public:
		RendererFonts();

		~RendererFonts();

		void Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
