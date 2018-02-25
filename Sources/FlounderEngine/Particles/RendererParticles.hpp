#pragma once

#include <vector>
#include "../Renderer/IRenderer.hpp"
#include "../Renderer/Buffers/UniformBuffer.hpp"
#include "../Renderer/Pipelines/Pipeline.hpp"
#include "Particles.hpp"

namespace Flounder
{
	class F_EXPORT RendererParticles :
		public IRenderer
	{
	private:
		static const int MAX_INSTANCES;

		UniformBuffer *m_uniformScene;
		UniformBuffer *m_uniformObject;
		Pipeline *m_pipeline;

		int m_rendered;
	public:
		RendererParticles(const GraphicsStage &graphicsStage);

		~RendererParticles();

		void Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;

	private:
		Matrix4 ModelMatrix(Particle *particle, const Matrix4 &viewMatrix);
	};
}
