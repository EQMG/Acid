#include "RendererMeshes.hpp"

#include "Renderer/Renderer.hpp"
#include "Models/Model.hpp"
#include "Scenes/Scenes.hpp"
#include "MeshRender.hpp"

namespace Flounder
{
	RendererMeshes::RendererMeshes(const GraphicsStage &graphicsStage) :
		IRenderer(),
		m_uniformScene(new UniformBuffer(64))
	{
	}

	RendererMeshes::~RendererMeshes()
	{
		delete m_uniformScene;
	}

	void RendererMeshes::Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		struct
		{
			Matrix4 projection;
			Matrix4 view;
		} uboScene;
		uboScene.projection = *camera.GetProjectionMatrix();
		uboScene.view = *camera.GetViewMatrix();
		m_uniformScene->Update(&uboScene);

		//m_uniformScene->UpdateMap("UniformScene", pipeline.GetShaderProgram(), {
		//	{"projection", *camera.GetProjectionMatrix()},
		//	{"view", *camera.GetViewMatrix()}
		//});

		std::vector<MeshRender *> renderList = std::vector<MeshRender *>();
		Scenes::Get()->GetStructure()->QueryComponents<MeshRender>(&renderList);

		for (auto entityRender : renderList)
		{
			entityRender->CmdRender(commandBuffer, m_uniformScene);
		}
	}
}
