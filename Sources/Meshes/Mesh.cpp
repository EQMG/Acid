#include "Mesh.hpp"

#include "Scenes/Entity.inl"
#include "Maths/Transform.hpp"
#include "Scenes/Scenes.hpp"
#include "Physics/Rigidbody.hpp"

namespace acid {
bool Mesh::registered = Register("mesh");

Mesh::Mesh(std::shared_ptr<Model> model, std::unique_ptr<Material> &&material) :
	m_model(std::move(model)),
	m_material(std::move(material)) {
}

void Mesh::Start() {
	if (m_material)
		m_material->Start(GetVertexInput(), false);
}

void Mesh::Update() {
	if (m_material) {
		auto transform = GetEntity()->GetComponent<Transform>();
		m_material->PushUniforms(m_uniformObject, transform);
	}
}

bool Mesh::CmdRender(const CommandBuffer &commandBuffer, UniformHandler &uniformScene, const Pipeline::Stage &pipelineStage) {
	if (!m_model || !m_material)
		return false;

	// Checks if the mesh is in view.
	/*if (auto rigidbody = GetEntity()->GetComponent<Rigidbody>()) {
		if (!rigidbody->InFrustum(Scenes::Get()->GetCamera()->GetViewFrustum()))
			return false;
	}*/

	// Check if we are in the correct pipeline stage.
	auto materialPipeline = m_material->GetPipelineMaterial();
	if (!materialPipeline || materialPipeline->GetStage() != pipelineStage)
		return false;

	// Binds the material pipeline.
	if (!materialPipeline->BindPipeline(commandBuffer))
		return false;

	const auto &pipeline = *materialPipeline->GetPipeline();

	// Updates descriptors.
	m_descriptorSet.Push("UniformScene", uniformScene);
	m_descriptorSet.Push("UniformObject", m_uniformObject);

	m_material->PushDescriptors(m_descriptorSet);

	if (!m_descriptorSet.Update(pipeline))
		return false;

	// Draws the object.
	m_descriptorSet.BindDescriptor(commandBuffer, pipeline);
	return m_model->CmdRender(commandBuffer);
}

void Mesh::SetMaterial(std::unique_ptr<Material> &&material) {
	m_material = std::move(material);
	m_material->Start(GetVertexInput(), false);
}

bool Mesh::operator<(const Mesh &other) const {
	auto camera = Scenes::Get()->GetCamera();

	auto transform0 = GetEntity()->GetComponent<Transform>();
	auto transform1 = other.GetEntity()->GetComponent<Transform>();

	auto thisDistance2 = (camera->GetPosition() - transform0->GetPosition()).LengthSquared();
	auto otherDistance2 = (camera->GetPosition() - transform1->GetPosition()).LengthSquared();

	return thisDistance2 > otherDistance2;
}

bool Mesh::operator>(const Mesh &other) const {
	return !operator<(other);
}

const Node &operator>>(const Node &node, Mesh &mesh) {
	node["model"].Get(mesh.m_model);
	node["material"].Get(mesh.m_material);
	return node;
}

Node &operator<<(Node &node, const Mesh &mesh) {
	node["model"].Set(mesh.m_model);
	node["material"].Set(mesh.m_material);
	return node;
}
}
