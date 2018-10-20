#pragma once

#include "Models/Model.hpp"
#include "Renderer/IRenderer.hpp"
#include "Renderer/Handlers/UniformHandler.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"
#include "Shadows.hpp"

namespace acid
{
	class ACID_EXPORT RendererShadows :
		public IRenderer
	{
	private:
		Pipeline m_pipeline;
		UniformHandler m_uniformScene;
	public:
		static const uint32_t NUM_CASCADES;

		static const float DEPTH_BIAS_CONSTANT;
		static const float DEPTH_BIAS_SLOPE;

		explicit RendererShadows(const GraphicsStage &graphicsStage);

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;

		std::vector<PipelineDefine> GetDefines();
	};
}
