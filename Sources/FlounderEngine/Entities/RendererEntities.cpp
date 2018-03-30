#include <Devices/Display.hpp>
#include "RendererEntities.hpp"

#include "../Renderer/Renderer.hpp"
#include "../Models/Model.hpp"
#include "../Scenes/Scenes.hpp"
#include "UbosEntities.hpp"
#include "EntityRender.hpp"

namespace Flounder
{
	RendererEntities::RendererEntities(const GraphicsStage &graphicsStage) :
		IRenderer(),
		m_uniformScene(new UniformBuffer(sizeof(UbosEntities::UboScene))),
		m_pipeline(new Pipeline(graphicsStage, PipelineCreate({"Resources/Shaders/Entities/Entity.vert", "Resources/Shaders/Entities/Entity.frag"},
			VertexModel::GetInputDescription(), PIPELINE_MRT, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT), { })) // {"ANIMATED"}, {"COLOUR_MAPPING"}, {"MATERIAL_MAPPING"}, {"NORMAL_MAPPING"}
	{
	}

	RendererEntities::~RendererEntities()
	{
		delete m_uniformScene;
		delete m_pipeline;
	}

	void RendererEntities::Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		UbosEntities::UboScene uboScene = {};
		uboScene.projection = *camera.GetProjectionMatrix();
		uboScene.view = *camera.GetViewMatrix();
		m_uniformScene->Update(&uboScene);

		//m_uniformScene->UpdateMap("UniformScene", pipeline.GetShaderProgram(), {
		//	{"projection", *camera.GetProjectionMatrix()},
		//	{"view", *camera.GetViewMatrix()}
		//});

		m_pipeline->BindPipeline(commandBuffer);

		std::vector<EntityRender *> renderList = std::vector<EntityRender *>();
		Scenes::Get()->GetStructure()->QueryComponents<EntityRender>(&renderList);

		for (auto entityRender : renderList)
		{
			entityRender->CmdRender(commandBuffer, *m_pipeline, m_uniformScene);
		}
	}
}
