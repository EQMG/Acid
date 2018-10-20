#include "RendererShadows.hpp"

#include "Models/VertexModel.hpp"
#include "Scenes/Scenes.hpp"
#include "ShadowRender.hpp"

namespace acid
{
	const uint32_t RendererShadows::NUM_CASCADES = 4;

	const float RendererShadows::DEPTH_BIAS_CONSTANT = 1.25f;
	const float RendererShadows::DEPTH_BIAS_SLOPE = 1.75f;

	RendererShadows::RendererShadows(const GraphicsStage &graphicsStage) :
		IRenderer(graphicsStage),
		m_pipeline(Pipeline(graphicsStage, PipelineCreate({"Shaders/Shadows/Shadow.vert", "Shaders/Shadows/Shadow.geom"}, {VertexModel::GetVertexInput()},
			PIPELINE_MODE_SHADOW, PIPELINE_DEPTH_READ_WRITE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_FRONT_BIT, GetDefines()))),
		m_uniformScene(UniformHandler())
	{
	}

	void RendererShadows::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		auto projectionViews = std::vector<Matrix4>(NUM_CASCADES);
		projectionViews[0] = Shadows::Get()->GetShadowBox().GetProjectionViewMatrix();
		projectionViews[1] = Shadows::Get()->GetShadowBox().GetProjectionViewMatrix();
		projectionViews[2] = Shadows::Get()->GetShadowBox().GetProjectionViewMatrix();
		projectionViews[3] = Shadows::Get()->GetShadowBox().GetProjectionViewMatrix();

		m_uniformScene.Push("projectionView", *projectionViews.data(), sizeof(Matrix4) * NUM_CASCADES);

		vkCmdSetDepthBias(commandBuffer.GetCommandBuffer(), DEPTH_BIAS_CONSTANT, 0.0f, DEPTH_BIAS_SLOPE);

		m_pipeline.BindPipeline(commandBuffer);

		/*auto sceneShadowRenders = Scenes::Get()->GetStructure()->QueryComponents<ShadowRender>();

		for (auto &shadowRender : sceneShadowRenders)
		{
			shadowRender->CmdRender(commandBuffer, m_pipeline, m_uniformScene);
		}*/
	}

	std::vector<PipelineDefine> RendererShadows::GetDefines()
	{
		std::vector<PipelineDefine> result = {};

		result.emplace_back(PipelineDefine("NUM_CASCADES", String::To(NUM_CASCADES)));

		return result;
	}
}
