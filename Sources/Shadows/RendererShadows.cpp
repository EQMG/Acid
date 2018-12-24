#include "RendererShadows.hpp"

#include "Models/VertexModel.hpp"
#include "Scenes/Scenes.hpp"
#include "ShadowRender.hpp"

namespace acid
{
	const uint32_t RendererShadows::NUM_CASCADES = 4; // TODO: Relocate to Shadows class.

	const float RendererShadows::DEPTH_BIAS_CONSTANT = 1.25f;
	const float RendererShadows::DEPTH_BIAS_SLOPE = 1.75f;

	RendererShadows::RendererShadows(const GraphicsStage &graphicsStage) :
		IRenderer(graphicsStage),
		m_pipeline(Pipeline(graphicsStage, PipelineCreate({"Shaders/Shadows/Shadow.vert", "Shaders/Shadows/Shadow.frag"}, {VertexModel::GetVertexInput()},
			PIPELINE_MODE_POLYGON, PIPELINE_DEPTH_NONE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_FRONT_BIT, GetDefines()))),
		m_uniformScene(UniformHandler())
	{
	}

	void RendererShadows::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const Camera &camera)
	{
		m_uniformScene.Push("projectionView", Shadows::Get()->GetShadowBox().GetProjectionViewMatrix());
		m_uniformScene.Push("cameraPosition", camera.GetPosition());

		vkCmdSetDepthBias(commandBuffer.GetCommandBuffer(), DEPTH_BIAS_CONSTANT, 0.0f, DEPTH_BIAS_SLOPE);

		m_pipeline.BindPipeline(commandBuffer);

		auto sceneShadowRenders = Scenes::Get()->GetStructure()->QueryComponents<ShadowRender>();

		for (auto &shadowRender : sceneShadowRenders)
		{
			shadowRender->CmdRender(commandBuffer, m_pipeline, m_uniformScene);
		}
	}

	std::vector<ShaderDefine> RendererShadows::GetDefines()
	{
		std::vector<ShaderDefine> result = {};
		result.emplace_back("NUM_CASCADES", String::To(NUM_CASCADES));
		return result;
	}
}
