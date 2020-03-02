#include "ParticlesSubrender.hpp"

#include "Scenes/Scenes.hpp"
#include "Models/Vertex3d.hpp"
#include "Particles.hpp"

namespace acid {
ParticlesSubrender::ParticlesSubrender(const Pipeline::Stage &pipelineStage) :
	Subrender(pipelineStage),
	pipeline(pipelineStage, {"Shaders/Particles/Particle.vert", "Shaders/Particles/Particle.frag"},
		{Vertex3d::GetVertexInput(0), ParticleType::Instance::GetVertexInput(1)}, {},
		PipelineGraphics::Mode::Polygon, PipelineGraphics::Depth::Read, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST) {
}

void ParticlesSubrender::Render(const CommandBuffer &commandBuffer) {
	auto camera = Scenes::Get()->GetCamera();
	uniformScene.Push("projection", camera->GetProjectionMatrix());
	uniformScene.Push("view", camera->GetViewMatrix());

	pipeline.BindPipeline(commandBuffer);

	auto particles = Particles::Get()->GetParticles();

	for (auto &[type, typeParticles] : particles) {
		type->CmdRender(commandBuffer, pipeline, uniformScene);
	}
}
}
