#include "Mesh.hpp"

#include "Scenes/Entity.hpp"
#include "Maths/Transform.hpp"
#include "Scenes/Scenes.hpp"

namespace acid {
Mesh::Mesh(std::shared_ptr<Model> model, std::unique_ptr<Material> &&material) :
	model(std::move(model)),
	material(std::move(material)) {
}

void Mesh::Start() {
	if (material)
		material->CreatePipeline(GetVertexInput(), false);
}

void Mesh::Update() {
	if (material) {
		auto transform = GetEntity()->GetComponent<Transform>();
		material->PushUniforms(uniformObject, transform);
	}
}

bool Mesh::CmdRender(const CommandBuffer &commandBuffer, UniformHandler &uniformScene, const Pipeline::Stage &pipelineStage) {
	if (!model || !material)
		return false;

	// Checks if the mesh is in view.
	if (auto rigidbody = GetEntity()->GetComponent<Rigidbody>()) {
		if (!rigidbody->InFrustum(Scenes::Get()->GetCamera()->GetViewFrustum()))
			return false;
	}

	// Check if we are in the correct pipeline stage.
	auto materialPipeline = material->GetPipelineMaterial();
	if (!materialPipeline || materialPipeline->GetStage() != pipelineStage)
		return false;

	// Binds the material pipeline.
	if (!materialPipeline->BindPipeline(commandBuffer))
		return false;

	const auto &pipeline = *materialPipeline->GetPipeline();

	// Updates descriptors.
	descriptorSet.Push("UniformScene", uniformScene);
	descriptorSet.Push("UniformObject", uniformObject);

	material->PushDescriptors(descriptorSet);

	if (!descriptorSet.Update(pipeline))
		return false;

	// Draws the object.
	descriptorSet.BindDescriptor(commandBuffer, pipeline);
	return model->CmdRender(commandBuffer);
}

void Mesh::SetMaterial(std::unique_ptr<Material> &&material) {
	this->material = std::move(material);
	this->material->CreatePipeline(GetVertexInput(), false);
}

bool Mesh::operator<(const Mesh &rhs) const {
	auto camera = Scenes::Get()->GetCamera();

	auto transform0 = GetEntity()->GetComponent<Transform>();
	auto transform1 = rhs.GetEntity()->GetComponent<Transform>();

	auto thisDistance2 = (camera->GetPosition() - transform0->GetPosition()).Length2();
	auto otherDistance2 = (camera->GetPosition() - transform1->GetPosition()).Length2();

	return thisDistance2 > otherDistance2;
}

bool Mesh::operator>(const Mesh &rhs) const {
	return !operator<(rhs);
}

const Node &operator>>(const Node &node, Mesh &mesh) {
	node["model"].Get(mesh.model);
	node["material"].Get(mesh.material);
	return node;
}

Node &operator<<(Node &node, const Mesh &mesh) {
	node["model"].Set(mesh.model);
	node["material"].Set(mesh.material);
	return node;
}
}
