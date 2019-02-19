#pragma once

#include <Models/Model.hpp>
#include <Renderer/RenderPipeline.hpp>
#include <Renderer/Pipelines/PipelineGraphics.hpp>
#include <Renderer/Handlers/DescriptorsHandler.hpp>
#include <Renderer/Handlers/StorageHandler.hpp>
#include <Renderer/Handlers/UniformHandler.hpp>
#include "Outline.hpp"

namespace acid
{
	class RendererFonts2 :
		public RenderPipeline
	{
	public:
		explicit RendererFonts2(const Pipeline::Stage &pipelineStage);

		void Render(const CommandBuffer &commandBuffer) override;

	private:
		PipelineGraphics m_pipeline;
		DescriptorsHandler m_descriptorSet;
	};
}
