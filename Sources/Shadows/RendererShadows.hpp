#pragma once

#include "Models/Model.hpp"
#include "Renderer/RenderPipeline.hpp"
#include "Renderer/Handlers/UniformHandler.hpp"
#include "Renderer/Pipelines/PipelineGraphics.hpp"
#include "Shadows.hpp"

namespace acid
{
	class ACID_EXPORT RendererShadows :
		public RenderPipeline
	{
	private:
		PipelineGraphics m_pipeline;
		UniformHandler m_uniformScene;
	public:
		static const uint32_t NUM_CASCADES;

		static const float DEPTH_BIAS_CONSTANT;
		static const float DEPTH_BIAS_SLOPE;

		explicit RendererShadows(const GraphicsStage &graphicsStage);

		void Render(const CommandBuffer &commandBuffer) override;
	private:
		std::vector<ShaderDefine> GetDefines();
	};
}
