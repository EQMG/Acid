#pragma once

#include <vector>
#include "Models/Model.hpp"
#include "Renderer/Buffers/UniformBuffer.hpp"
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
		RendererParticles(const GraphicsStage &graphicsStage);

		~RendererParticles();

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;

	private:
		Matrix4 ModelMatrix(Particle *particle, const Matrix4 &viewMatrix);
	};
}
