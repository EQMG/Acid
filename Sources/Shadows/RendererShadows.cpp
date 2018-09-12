#include "RendererShadows.hpp"

#include "Models/VertexModel.hpp"
#include "Scenes/Scenes.hpp"
#include "ShadowRender.hpp"

namespace acid
{
	RendererShadows::RendererShadows(const GraphicsStage &graphicsStage) :
		IRenderer(graphicsStage),
		m_pipeline(Pipeline(graphicsStage, PipelineCreate({"Shaders/Shadows/Shadow.vert", "Shaders/Shadows/Shadow.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MODE_POLYGON, PIPELINE_DEPTH_NONE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_FRONT_BIT, {}))),
		m_uniformScene(UniformHandler())
	{
	}

	RendererShadows::~RendererShadows()
	{
	}

	void RendererShadows::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		m_uniformScene.Push("projectionView", Shadows::Get()->GetShadowBox().GetProjectionViewMatrix());
		m_uniformScene.Push("cameraPosition", camera.GetPosition());

		m_pipeline.BindPipeline(commandBuffer);

		auto sceneShadowRenders = Scenes::Get()->GetStructure()->QueryComponents<ShadowRender>();

		for (auto &shadowRender : sceneShadowRenders)
		{
			shadowRender->CmdRender(commandBuffer, m_pipeline, m_uniformScene);
		}
	}
}
