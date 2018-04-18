#include "RendererEntities.hpp"

#include "Renderer/Renderer.hpp"
#include "Models/Model.hpp"
#include "Scenes/Scenes.hpp"
#include "EntityRender.hpp"

namespace Flounder
{
	RendererEntities::RendererEntities(const GraphicsStage &graphicsStage) :
		IRenderer(),
		m_uniformScene(new UniformHandler())
	{
	}

	RendererEntities::~RendererEntities()
	{
		delete m_uniformScene;
	}

	void RendererEntities::Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		m_uniformScene->Push("projection", *camera.GetProjectionMatrix());
		m_uniformScene->Push("view", *camera.GetViewMatrix());

		std::vector<EntityRender *> renderList = std::vector<EntityRender *>();
		Scenes::Get()->GetStructure()->QueryComponents<EntityRender>(&renderList);

		for (auto entityRender : renderList)
		{
			entityRender->CmdRender(commandBuffer, m_uniformScene);
		}
	}
}
