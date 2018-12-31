#include "RendererParticles.hpp"

#include "Maths/Maths.hpp"
#include "Models/VertexModel.hpp"

namespace acid
{
	RendererParticles::RendererParticles(const GraphicsStage &graphicsStage) :
		RenderPipeline(graphicsStage),
		m_pipeline(PipelineGraphics(graphicsStage, {"Shaders/Particles/Particle.vert", "Shaders/Particles/Particle.frag"}, {VertexModel::GetVertexInput()},
			PIPELINE_MODE_POLYGON, PIPELINE_DEPTH_READ, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, false, {})),
		m_uniformScene(UniformHandler())
	{
	}

	void RendererParticles::Render(const CommandBuffer &commandBuffer, const Camera &camera)
	{
		m_uniformScene.Push("projection", camera.GetProjectionMatrix());
		m_uniformScene.Push("view", camera.GetViewMatrix());

		auto particles = Particles::Get()->GetParticles();

		m_pipeline.BindPipeline(commandBuffer);

		for (auto &[type, particles] : particles)
		{
			type->CmdRender(commandBuffer, m_pipeline, m_uniformScene);
		}
	}
}
