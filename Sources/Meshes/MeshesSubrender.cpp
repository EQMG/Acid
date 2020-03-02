#include "MeshesSubrender.hpp"

#include "Animations/AnimatedMesh.hpp"
#include "Scenes/Scenes.hpp"
#include "Mesh.hpp"

namespace acid {
MeshesSubrender::MeshesSubrender(const Pipeline::Stage &pipelineStage, Sort sort) :
	Subrender(pipelineStage),
	sort(sort),
	uniformScene(true) {
}

void MeshesSubrender::Render(const CommandBuffer &commandBuffer) {
	auto camera = Scenes::Get()->GetCamera();
	uniformScene.Push("projection", camera->GetProjectionMatrix());
	uniformScene.Push("view", camera->GetViewMatrix());
	uniformScene.Push("cameraPos", camera->GetPosition());

	auto meshes = Scenes::Get()->GetStructure()->QueryComponents<Mesh>();
	if (sort == Sort::Front)
		std::sort(meshes.begin(), meshes.end(), std::greater<>());
	else if (sort == Sort::Back)
		std::sort(meshes.begin(), meshes.end(), std::less<>());

	for (const auto &mesh : meshes) {
		mesh->CmdRender(commandBuffer, uniformScene, GetStage());
	}

	// TODO: Split animated meshes into it's own subrender.
	auto animatedMeshes = Scenes::Get()->GetStructure()->QueryComponents<AnimatedMesh>();
	for (const auto &animatedMesh : animatedMeshes) {
		animatedMesh->CmdRender(commandBuffer, uniformScene, GetStage());
	}
}
}
