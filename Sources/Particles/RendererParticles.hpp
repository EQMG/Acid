#pragma once

#include <vector>
#include "Renderer/IRenderer.hpp"
#include "Renderer/Handlers/UniformHandler.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"
#include "Particles.hpp"

namespace acid
{
	class ACID_EXPORT RendererParticles :
		public IRenderer
	{
	private:
		Pipeline m_pipeline;
		UniformHandler m_uniformScene;
	public:
		explicit RendererParticles(const GraphicsStage &graphicsStage);

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const Camera &camera) override;
	};
}
