#include "RendererSkyboxes.hpp"

#include "Scenes/Scenes.hpp"
#include "Models/Model.hpp"
#include "SkyboxRender.hpp"

namespace Flounder
{
	RendererSkyboxes::RendererSkyboxes(const GraphicsStage &graphicsStage) :
		IRenderer(),
		m_pipeline(new Pipeline(graphicsStage, PipelineCreate({"Resources/Shaders/Skyboxes/Skybox.vert", "Resources/Shaders/Skyboxes/Skybox.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MRT_NO_DEPTH, VK_POLYGON_MODE_FILL, VK_CULL_MODE_FRONT_BIT), {})),
		m_uniformScene(new UniformHandler())
	{
	}

	RendererSkyboxes::~RendererSkyboxes()
	{
		delete m_pipeline;
		delete m_uniformScene;
	}

	void RendererSkyboxes::Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		Matrix4 view = Matrix4(*camera.GetViewMatrix());
		view.m_30 = 0.0f;
		view.m_31 = 0.0f;
		view.m_32 = 0.0f;
		m_uniformScene->Push("projection", *camera.GetProjectionMatrix());
		m_uniformScene->Push("view", view);

		m_pipeline->BindPipeline(commandBuffer);

		std::vector<SkyboxRender *> renderList = std::vector<SkyboxRender *>();
		Scenes::Get()->GetStructure()->QueryComponents(&renderList);

		for (auto skyboxRender : renderList)
		{
			skyboxRender->CmdRender(commandBuffer, *m_pipeline, m_uniformScene);
		}
	}
}
