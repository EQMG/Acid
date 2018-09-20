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
		UniformHandler m_uniformScene;
		Pipeline m_pipeline;
	public:
		explicit RendererParticles(const GraphicsStage &graphicsStage);

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	private:
		ParticleData GetInstanceData(Particle &particle, const Matrix4 &viewMatrix);
	};
}
