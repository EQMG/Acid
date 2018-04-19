#include "RendererMeshes.hpp"

#include "Renderer/Renderer.hpp"
#include "Scenes/Scenes.hpp"
#include "MeshRender.hpp"

namespace Flounder
{
	RendererMeshes::RendererMeshes(const GraphicsStage &graphicsStage) :
		IRenderer(),
		m_uniformScene(new UniformHandler())
	{
	}

	RendererMeshes::~RendererMeshes()
	{
		delete m_uniformScene;
	}

	void RendererMeshes::Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		m_uniformScene->Push("projection", *camera.GetProjectionMatrix());
		m_uniformScene->Push("view", *camera.GetViewMatrix());

		std::vector<MeshRender *> renderList = std::vector<MeshRender *>();
		Scenes::Get()->GetStructure()->QueryComponents<MeshRender>(&renderList);

		for (auto meshRender : renderList)
		{
			meshRender->CmdRender(commandBuffer, m_uniformScene);
		}
	}
}
