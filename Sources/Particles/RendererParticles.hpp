#pragma once

#include <vector>
#include "Renderer/IRenderer.hpp"
#include "Renderer/Buffers/UniformBuffer.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"
#include "Models/Model.hpp"
#include "Particles.hpp"

namespace fl
{
	class FL_EXPORT RendererParticles :
		public IRenderer
	{
	private:
		UniformHandler *m_uniformScene;
		Pipeline *m_pipeline;
	public:
		RendererParticles(const GraphicsStage &graphicsStage);

		~RendererParticles();

		void Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;

	private:
		Matrix4 ModelMatrix(Particle *particle, const Matrix4 &viewMatrix);
	};
}
