#include "RendererMeshes.hpp"

#include "Scenes/Scenes.hpp"
#include "MeshRender.hpp"

namespace acid
{
	RendererMeshes::RendererMeshes(const GraphicsStage &graphicsStage, const MeshSort &meshSort) :
		IRenderer(graphicsStage),
		m_meshSort(meshSort),
		m_uniformScene(UniformHandler(true))
	{
	}

	void RendererMeshes::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const Camera &camera)
	{
		m_uniformScene.Push("projection", camera.GetProjectionMatrix());
		m_uniformScene.Push("view", camera.GetViewMatrix());
		m_uniformScene.Push("cameraPos", camera.GetPosition());

		auto sceneMeshRenders = Scenes::Get()->GetStructure()->QueryComponents<MeshRender>();

		if (m_meshSort != MESH_SORT_NONE)
		{
			std::sort(sceneMeshRenders.begin(), sceneMeshRenders.end());

			if (m_meshSort == MESH_SORT_FRONT)
			{
				std::reverse(sceneMeshRenders.begin(), sceneMeshRenders.end());
			}
		}

		for (auto &meshRender : sceneMeshRenders)
		{
			meshRender->CmdRender(commandBuffer, m_uniformScene, GetGraphicsStage());
		}
	}
}
