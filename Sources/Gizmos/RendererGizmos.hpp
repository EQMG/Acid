#pragma once

#include "Renderer/IRenderer.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"
#include "Renderer/Handlers/UniformHandler.hpp"

namespace acid
{
	class ACID_EXPORT RendererGizmos :
		public IRenderer
	{
	private:
		Pipeline m_pipeline;
		UniformHandler m_uniformScene;
	public:
		explicit RendererGizmos(const GraphicsStage &graphicsStage);

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const Camera &camera) override;
	};
}
