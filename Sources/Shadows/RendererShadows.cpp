#include "RendererShadows.hpp"

#include "Models/VertexModel.hpp"
#include "Scenes/Scenes.hpp"
#include "ShadowRender.hpp"
#include "Shadows.hpp"

namespace acid
{
const uint32_t RendererShadows::Cascades = 4; // TODO: Relocate to Shadows class.

const float RendererShadows::BiasConstants = 1.25f;
const float RendererShadows::BiasSlope = 1.75f;

RendererShadows::RendererShadows(const Pipeline::Stage& pipelineStage)
	: RenderPipeline(pipelineStage), m_pipeline(pipelineStage, {"Shaders/Shadows/Shadow.vert", "Shaders/Shadows/Shadow.frag"}, {VertexModel::GetVertexInput()}, GetDefines(), PipelineGraphics::Mode::Polygon, PipelineGraphics::Depth::None, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
												VK_POLYGON_MODE_FILL, VK_CULL_MODE_FRONT_BIT)
{
}

void RendererShadows::Render(const CommandBuffer& commandBuffer)
{
	auto camera = Scenes::Get()->GetCamera();
	m_uniformScene.Push("projectionView", Shadows::Get()->GetShadowBox().GetProjectionViewMatrix());
	m_uniformScene.Push("cameraPosition", camera->GetPosition());

	vkCmdSetDepthBias(commandBuffer.GetCommandBuffer(), BiasConstants, 0.0f, BiasSlope);

	m_pipeline.BindPipeline(commandBuffer);

	//	vkCmdSetDepthBias(commandBuffer.GetCommandBuffer(),
	// 1.25f,
	// 0.0f,
	// 1.75f);

	auto sceneShadowRenders = Scenes::Get()->GetStructure()->QueryComponents<ShadowRender>();

	for(const auto& shadowRender : sceneShadowRenders)
		{
			shadowRender->CmdRender(commandBuffer, m_pipeline, m_uniformScene);
		}
}

std::vector<Shader::Define> RendererShadows::GetDefines()
{
	std::vector<Shader::Define> result = {};
	result.emplace_back("NUM_CASCADES", String::To(Cascades));
	return result;
}
}
