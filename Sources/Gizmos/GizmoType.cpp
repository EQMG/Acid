#include "GizmoType.hpp"

#include "Resources/Resources.hpp"
#include "Scenes/Scenes.hpp"
#include "Gizmo.hpp"

namespace acid {
static const uint32_t MAX_INSTANCES = 512;
//static const uint32_t INSTANCE_STEPS = 128;
//static const float FRUSTUM_BUFFER = 1.4f;

std::shared_ptr<GizmoType> GizmoType::Create(const Node &node) {
	if (auto resource = Resources::Get()->Find<GizmoType>(node))
		return resource;

	auto result = std::make_shared<GizmoType>(nullptr);
	Resources::Get()->Add(node, std::dynamic_pointer_cast<Resource>(result));
	node >> *result;
	//result->Load();
	return result;
}

std::shared_ptr<GizmoType> GizmoType::Create(const std::shared_ptr<Model> &model, float lineThickness, const Colour &colour) {
	GizmoType temp(model, lineThickness, colour);
	Node node;
	node << temp;
	return Create(node);
}

GizmoType::GizmoType(std::shared_ptr<Model> model, float lineThickness, const Colour &colour) :
	model(std::move(model)),
	lineThickness(lineThickness),
	colour(colour),
	instanceBuffer(sizeof(Instance) * MAX_INSTANCES) {
}

void GizmoType::Update(const std::vector<std::unique_ptr<Gizmo>> &gizmos) {
	// Calculates a max instance count over the time of the type. TODO: Allow decreasing max using a timer and average count over the delay.
	//uint32_t instances = INSTANCE_STEPS * static_cast<uint32_t>(std::ceil(static_cast<float>(gizmos.size()) / static_cast<float>(INSTANCE_STEPS)));
	//maxInstances = std::max(maxInstances, instances);
	maxInstances = MAX_INSTANCES;
	instances = 0;

	if (gizmos.empty())
		return;

	Instance *instances;
	instanceBuffer.MapMemory(reinterpret_cast<void **>(&instances));

	for (const auto &gizmo : gizmos) {
		if (this->instances >= maxInstances)
			break;

		//if (!Scenes::Get()->GetCamera()->GetViewFrustum().SphereInFrustum(gizmo->GetTransform().GetPosition(), FRUSTUM_BUFFER * gizmo->GetTransform().GetPosition().GetScale())) {
		//	continue;
		//}

		auto instance = &instances[this->instances];
		instance->modelMatrix = gizmo->transform.GetWorldMatrix();
		instance->colour = gizmo->colour;
		this->instances++;
	}

	instanceBuffer.UnmapMemory();
}

bool GizmoType::CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline, UniformHandler &uniformScene) {
	if (instances == 0)
		return false;

	// Updates descriptors.
	descriptorSet.Push("UniformScene", uniformScene);

	if (!descriptorSet.Update(pipeline))
		return false;

	vkCmdSetLineWidth(commandBuffer, lineThickness);

	// Draws the instanced objects.
	descriptorSet.BindDescriptor(commandBuffer, pipeline);

	VkBuffer vertexBuffers[2] = {model->GetVertexBuffer()->GetBuffer(), instanceBuffer.GetBuffer()};
	VkDeviceSize offsets[2] = {0, 0};
	vkCmdBindVertexBuffers(commandBuffer, 0, 2, vertexBuffers, offsets);
	vkCmdBindIndexBuffer(commandBuffer, model->GetIndexBuffer()->GetBuffer(), 0, model->GetIndexType());
	vkCmdDrawIndexed(commandBuffer, model->GetIndexCount(), instances, 0, 0, 0);
	return true;
}

const Node &operator>>(const Node &node, GizmoType &gizmoType) {
	node["model"].Get(gizmoType.model);
	node["lineThickness"].Get(gizmoType.lineThickness);
	node["colour"].Get(gizmoType.colour);
	return node;
}

Node &operator<<(Node &node, const GizmoType &gizmoType) {
	node["model"].Set(gizmoType.model);
	node["lineThickness"].Set(gizmoType.lineThickness);
	node["colour"].Set(gizmoType.colour);
	return node;
}
}
