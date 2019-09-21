#include "MeshRender.hpp"

#include "Animations/MeshAnimated.hpp"
#include "Materials/Material.hpp"
#include "Physics/Rigidbody.hpp"
#include "Maths/Transform.hpp"
#include "Scenes/Entity.hpp"
#include "Scenes/Scenes.hpp"

namespace acid {
void MeshRender::Start() {
}

void MeshRender::Update() {
	auto material = GetEntity()->GetComponent<Material>();

	if (!material) {
		return;
	}

	// Updates uniforms.
	material->PushUniforms(m_uniformObject);
}

bool MeshRender::CmdRender(const CommandBuffer &commandBuffer, UniformHandler &uniformScene, const Pipeline::Stage &pipelineStage) {
	// Checks if the mesh is in view.
	if (auto rigidbody = GetEntity()->GetComponent<Rigidbody>(); rigidbody) {
		if (!rigidbody->InFrustum(Scenes::Get()->GetCamera()->GetViewFrustum())) {
			return false;
		}
	}

	// Gets required components.
	auto material = GetEntity()->GetComponent<Material>();
	auto mesh = GetEntity()->GetComponent<Mesh>();
	auto meshAnimated = GetEntity()->GetComponent<MeshAnimated>();

	if (!material || (!mesh && !meshAnimated))
	{
		return false;
	}

	auto meshModel = meshAnimated ? meshAnimated->GetModel() : mesh->GetModel(); 
	auto materialPipeline = material->GetPipelineMaterial();

	if (!meshModel || !materialPipeline || materialPipeline->GetStage() != pipelineStage) {
		return false;
	}

	// Binds the material pipeline.

	if (!materialPipeline->BindPipeline(commandBuffer)) {
		return false;
	}

	auto &pipeline = *materialPipeline->GetPipeline();

	// Updates descriptors.
	m_descriptorSet.Push("UniformScene", uniformScene);
	m_descriptorSet.Push("UniformObject", m_uniformObject);
	material->PushDescriptors(m_descriptorSet);

	if (!m_descriptorSet.Update(pipeline)) {
		return false;
	}

	// Draws the object.
	m_descriptorSet.BindDescriptor(commandBuffer, pipeline);
	return meshModel->CmdRender(commandBuffer);
}

bool MeshRender::operator<(const MeshRender &other) const {
	auto camera = Scenes::Get()->GetCamera();

	auto transform0 = GetEntity()->GetComponent<Transform>();
	auto transform1 = other.GetEntity()->GetComponent<Transform>();

	auto thisDistance2 = (camera->GetPosition() - transform0->GetPosition()).LengthSquared();
	auto otherDistance2 = (camera->GetPosition() - transform1->GetPosition()).LengthSquared();

	return thisDistance2 > otherDistance2;
}

const Node &operator>>(const Node &node, MeshRender &meshRender) {
	return node;
}

Node &operator<<(Node &node, const MeshRender &meshRender) {
	return node;
}
}
