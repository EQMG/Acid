#include "RendererMeshes.hpp"

#include "Renderer/Renderer.hpp"
#include "Scenes/Scenes.hpp"
#include "MeshRender.hpp"

namespace fl
{
	RendererMeshes::RendererMeshes(const GraphicsStage &graphicsStage) :
		IRenderer(graphicsStage),
		m_uniformScene(UniformHandler(true))
	{
	}

	RendererMeshes::~RendererMeshes()
	{
	}

	void RendererMeshes::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		m_uniformScene.Push("projection", camera.GetProjectionMatrix());
		m_uniformScene.Push("view", camera.GetViewMatrix());

		auto renderList = Scenes::Get()->GetStructure()->QueryComponents<MeshRender>();

		for (auto &meshRender : renderList)
		{
			meshRender->CmdRender(commandBuffer, m_uniformScene);
		}
	}
}
