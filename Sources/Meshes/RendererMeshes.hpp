#pragma once

#include "Renderer/IRenderer.hpp"
#include "Renderer/Buffers/UniformBuffer.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"

namespace fl
{
	class FL_EXPORT RendererMeshes :
		public IRenderer
	{
	private:
		UniformHandler *m_uniformScene;
	public:
		RendererMeshes(const GraphicsStage &graphicsStage);

		~RendererMeshes();

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
