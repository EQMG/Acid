#include "RendererShadows.hpp"

#include "Models/VertexModel.hpp"
#include "Scenes/Scenes.hpp"
#include "ShadowRender.hpp"

namespace acid
{
	const uint32_t RendererShadows::Cascades = 4; // TODO: Relocate to Shadows class.

	const float RendererShadows::BiasConstants = 1.25f;
	const float RendererShadows::BiasSlope = 1.75f;

	RendererShadows::RendererShadows(const GraphicsStage &graphicsStage) :
		RenderPipeline(graphicsStage),
		m_pipeline(PipelineGraphics(graphicsStage, PipelineCreate({"Shaders/Shadows/Shadow.vert", "Shaders/Shadows/Shadow.frag"}, {VertexModel::GetVertexInput()},
			PipelineMode::Polygon, PipelineDepth::None, VK_POLYGON_MODE_FILL, VK_CULL_MODE_FRONT_BIT, false, GetDefines()))),
		m_uniformScene(UniformHandler())
	{
	}

	void RendererShadows::Render(const CommandBuffer &commandBuffer)
	{
		auto camera = Scenes::Get()->GetCamera();
		m_uniformScene.Push("projectionView", Shadows::Get()->GetShadowBox().GetProjectionViewMatrix());
		m_uniformScene.Push("cameraPosition", camera->GetPosition());

		vkCmdSetDepthBias(commandBuffer.GetCommandBuffer(), BiasConstants, 0.0f, BiasSlope);

		m_pipeline.BindPipeline(commandBuffer);

		auto sceneShadowRenders = Scenes::Get()->GetStructure()->QueryComponents<ShadowRender>();

		for (const auto &shadowRender : sceneShadowRenders)
		{
			shadowRender->CmdRender(commandBuffer, m_pipeline, m_uniformScene);
		}
	}

	std::vector<ShaderDefine> RendererShadows::GetDefines()
	{
		std::vector<ShaderDefine> result = {};
		result.emplace_back("NUM_CASCADES", String::To(Cascades));
		return result;
	}
}
