#include "RendererShadows.hpp"

#include "Models/VertexModel.hpp"
#include "Scenes/Scenes.hpp"
#include "ShadowRender.hpp"
#include "Shadows.hpp"

namespace acid
{
RendererShadows::RendererShadows(const Pipeline::Stage &pipelineStage) :
	RenderPipeline(pipelineStage),
	m_pipeline(pipelineStage, { "Shaders/Shadows/Shadow.vert", "Shaders/Shadows/Shadow.frag" }, { VertexModel::GetVertexInput() }, GetDefines(), PipelineGraphics::Mode::Polygon,
		PipelineGraphics::Depth::None, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_POLYGON_MODE_FILL, VK_CULL_MODE_FRONT_BIT)
{
}

void RendererShadows::Render(const CommandBuffer &commandBuffer)
{
	auto camera = Scenes::Get()->GetCamera();

	m_pipeline.BindPipeline(commandBuffer);

	auto sceneShadowRenders = Scenes::Get()->GetStructure()->QueryComponents<ShadowRender>();

	for (const auto &shadowRender : sceneShadowRenders)
	{
		shadowRender->CmdRender(commandBuffer, m_pipeline);
	}
}

std::vector<Shader::Define> RendererShadows::GetDefines()
{
	std::vector<Shader::Define> defines;
	return defines;
}
}
