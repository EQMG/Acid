#include "ShadowRender.hpp"

#include "Maths/Transform.hpp"
#include "Meshes/Mesh.hpp"
#include "Scenes/Entity.hpp"
#include "Shadows.hpp"

namespace acid {
ShadowRender::ShadowRender() {
}

void ShadowRender::Start() {
}

void ShadowRender::Update() {
}

bool ShadowRender::CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline) {
	auto transform = GetEntity()->GetComponent<Transform>();

	if (!transform)
		return false;

	// Update push constants.
	pushObject.Push("mvp", Shadows::Get()->GetShadowBox().GetProjectionViewMatrix() * transform->GetWorldMatrix());

	// Gets required components.
	auto mesh = GetEntity()->GetComponent<Mesh>();

	if (!mesh || !mesh->GetModel())
		return false;

	// Updates descriptors.
	descriptorSet.Push("PushObject", pushObject);

	if (!descriptorSet.Update(pipeline))
		return false;

	// Draws the object.
	descriptorSet.BindDescriptor(commandBuffer, pipeline);
	pushObject.BindPush(commandBuffer, pipeline);
	return mesh->GetModel()->CmdRender(commandBuffer);
}

const Node &operator>>(const Node &node, ShadowRender &shadowRender) {
	return node;
}

Node &operator<<(Node &node, const ShadowRender &shadowRender) {
	return node;
}
}
