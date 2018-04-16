#include "RendererSkyboxes.hpp"

#include "Scenes/Scenes.hpp"
#include "Models/Model.hpp"
#include "UbosSkyboxes.hpp"
#include "SkyboxRender.hpp"

namespace Flounder
{
	RendererSkyboxes::RendererSkyboxes(const GraphicsStage &graphicsStage) :
		IRenderer(),
		m_uniformScene(new UniformBuffer(sizeof(UbosSkyboxes::UboScene))),
		m_pipeline(new Pipeline(graphicsStage, PipelineCreate({"Resources/Shaders/Skyboxes/Skybox.vert", "Resources/Shaders/Skyboxes/Skybox.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MRT_NO_DEPTH, VK_POLYGON_MODE_FILL, VK_CULL_MODE_FRONT_BIT), {}))
	{
	}

	RendererSkyboxes::~RendererSkyboxes()
	{
		delete m_uniformScene;
		delete m_pipeline;
	}

	void RendererSkyboxes::Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		UbosSkyboxes::UboScene uboScene = {};
		uboScene.projection = *camera.GetProjectionMatrix();
		uboScene.view = *camera.GetViewMatrix();
		uboScene.view.m_30 = 0.0f;
		uboScene.view.m_31 = 0.0f;
		uboScene.view.m_32 = 0.0f;
		m_uniformScene->Update(&uboScene);

		m_pipeline->BindPipeline(commandBuffer);

		std::vector<SkyboxRender *> renderList = std::vector<SkyboxRender *>();
		Scenes::Get()->GetStructure()->QueryComponents(&renderList);

		for (auto skyboxRender : renderList)
		{
			skyboxRender->CmdRender(commandBuffer, *m_pipeline, m_uniformScene);
		}
	}
}
