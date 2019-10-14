#include "SubrenderMeshes.hpp"

#include "Animations/MeshAnimated.hpp"
#include "Scenes/Scenes.hpp"
#include "Mesh.hpp"

namespace acid {
SubrenderMeshes::SubrenderMeshes(const Pipeline::Stage &pipelineStage, Sort sort) :
	Subrender(pipelineStage),
	m_sort(sort),
	m_uniformScene(true) {
}

void SubrenderMeshes::Render(const CommandBuffer &commandBuffer) {
	auto camera = Scenes::Get()->GetCamera();
	m_uniformScene.Push("projection", camera->GetProjectionMatrix());
	m_uniformScene.Push("view", camera->GetViewMatrix());
	m_uniformScene.Push("cameraPos", camera->GetPosition());

	/*auto meshes = Scenes::Get()->GetStructure()->QueryComponents<Mesh>();
	if (m_sort == Sort::Front)
		std::sort(meshes.begin(), meshes.end(), std::greater<>());
	else if (m_sort == Sort::Back)
		std::sort(meshes.begin(), meshes.end(), std::less<>());

	for (const auto &mesh : meshes) {
		mesh->CmdRender(commandBuffer, m_uniformScene, GetStage());
	}

	// TODO: Split animated meshes into it's own subrender.
	auto animatedMeshes = Scenes::Get()->GetStructure()->QueryComponents<MeshAnimated>();
	for (const auto &animatedMesh : animatedMeshes) {
		animatedMesh->CmdRender(commandBuffer, m_uniformScene, GetStage());
	}*/
}
}
