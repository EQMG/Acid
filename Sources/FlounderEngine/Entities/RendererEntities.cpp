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
		m_uniformScene(new UniformBuffer(sizeof(UbosEntities::UboScene)))
	{
	}

	RendererEntities::~RendererEntities()
	{
		delete m_uniformScene;
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

		std::vector<EntityRender *> renderList = std::vector<EntityRender *>();
		Scenes::Get()->GetStructure()->QueryComponents<EntityRender>(&renderList);

		for (auto entityRender : renderList)
		{
			entityRender->CmdRender(commandBuffer, m_uniformScene);
		}
	}
}
