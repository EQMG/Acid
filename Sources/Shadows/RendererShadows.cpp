#include "RendererShadows.hpp"

#include "Scenes/Scenes.hpp"
#include "ShadowRender.hpp"

namespace fl
{
	RendererShadows::RendererShadows(const GraphicsStage &graphicsStage) :
		IRenderer(graphicsStage),
		m_pipeline(Pipeline(graphicsStage, PipelineCreate({"Shaders/Shadows/Shadow.vert", "Shaders/Shadows/Shadow.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MODE_POLYGON_NO_DEPTH, PIPELINE_POLYGON_MODE_FILL, PIPELINE_CULL_MODE_FRONT), {})),
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

		auto renderList = Scenes::Get()->GetStructure()->QueryComponents<ShadowRender>();

		for (auto &shadowRender : renderList)
		{
			shadowRender->CmdRender(commandBuffer, m_pipeline, m_uniformScene);
		}
	}
}
