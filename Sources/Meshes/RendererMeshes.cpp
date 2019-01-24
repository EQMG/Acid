#include "RendererMeshes.hpp"

#include "Scenes/Scenes.hpp"
#include "MeshRender.hpp"

namespace acid
{
	RendererMeshes::RendererMeshes(const Pipeline::Stage &pipelineStage, const Sort &sort) :
		RenderPipeline(pipelineStage),
		m_sort(sort),
		m_uniformScene(UniformHandler(true))
	{
	}

	void RendererMeshes::Render(const CommandBuffer &commandBuffer)
	{
		auto camera = Scenes::Get()->GetCamera();
		m_uniformScene.Push("projection", camera->GetProjectionMatrix());
		m_uniformScene.Push("view", camera->GetViewMatrix());
		m_uniformScene.Push("cameraPos", camera->GetPosition());

		auto sceneMeshRenders = Scenes::Get()->GetStructure()->QueryComponents<MeshRender>();

		if (m_sort != Sort::None)
		{
			std::sort(sceneMeshRenders.begin(), sceneMeshRenders.end());

			if (m_sort == Sort::Front)
			{
				std::reverse(sceneMeshRenders.begin(), sceneMeshRenders.end());
			}
		}

		for (const auto &meshRender : sceneMeshRenders)
		{
			meshRender->CmdRender(commandBuffer, m_uniformScene, GetStage());
		}
	}
}
