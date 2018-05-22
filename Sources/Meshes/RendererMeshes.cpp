#include "RendererMeshes.hpp"

#include "Renderer/Renderer.hpp"
#include "Scenes/Scenes.hpp"
#include "MeshRender.hpp"

namespace fl
{
	RendererMeshes::RendererMeshes(const GraphicsStage &graphicsStage) :
		IRenderer()
	{
	}

	RendererMeshes::~RendererMeshes()
	{
	}

	void RendererMeshes::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		std::vector<MeshRender *> renderList = std::vector<MeshRender *>();
		Scenes::Get()->GetStructure()->QueryComponents<MeshRender>(&renderList);

		for (auto meshRender : renderList)
		{
			meshRender->CmdRender(commandBuffer);
		}
	}
}
