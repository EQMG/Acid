#include "RendererShadows.hpp"

#include "Scenes/Scenes.hpp"
#include "ShadowRender.hpp"

namespace fl
{
	RendererShadows::RendererShadows(const GraphicsStage &graphicsStage) :
		IRenderer(),
		m_pipeline(new Pipeline(graphicsStage, PipelineCreate({"Resources/Shaders/Shadows/Shadow.vert", "Resources/Shaders/Shadows/Shadow.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MODE_POLYGON_NO_DEPTH, PIPELINE_POLYGON_MODE_FILL, PIPELINE_CULL_MODE_FRONT), {})),
		m_uniformScene(new UniformHandler())
	{
	}

	RendererShadows::~RendererShadows()
	{
		delete m_pipeline;
		delete m_uniformScene;
	}

	void RendererShadows::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		m_uniformScene->Push("projectionView", Shadows::Get()->GetShadowBox().GetProjectionViewMatrix());
		m_uniformScene->Push("cameraPosition", camera.GetPosition());

		m_pipeline->BindPipeline(commandBuffer);

		std::vector<ShadowRender *> renderList = std::vector<ShadowRender *>();
		Scenes::Get()->GetStructure()->QueryComponents<ShadowRender>(&renderList);

		for (auto shadowRender : renderList)
		{
			shadowRender->CmdRender(commandBuffer, *m_pipeline, m_uniformScene);
		}
	}
}
