#include "GizmoType.hpp"

#include "Resources/Resources.hpp"
#include "Scenes/Scenes.hpp"
#include "Gizmo.hpp"

namespace acid
{
static const uint32_t MAX_INSTANCES{512};
//static const uint32_t INSTANCE_STEPS{128};
//static const float FRUSTUM_BUFFER{1.4f};

std::shared_ptr<GizmoType> GizmoType::Create(const Node &node)
{
	auto resource{Resources::Get()->Find(node)};

	if (resource != nullptr)
	{
		return std::dynamic_pointer_cast<GizmoType>(resource);
	}

	auto result{std::make_shared<GizmoType>(nullptr)};
	Resources::Get()->Add(node, std::dynamic_pointer_cast<Resource>(result));
	node >> *result;
	//result->Load();
	return result;
}

std::shared_ptr<GizmoType> GizmoType::Create(const std::shared_ptr<Model> &model, const float &lineThickness, const Colour &colour)
{
	GizmoType temp{model, lineThickness, colour};
	Node node;
	node << temp;
	return Create(node);
}

GizmoType::GizmoType(std::shared_ptr<Model> model, const float &lineThickness, const Colour &colour) :
	m_model{std::move(model)},
	m_lineThickness{lineThickness},
	m_colour{colour},
	m_instanceBuffer{sizeof(Instance) * MAX_INSTANCES}
{
}

void GizmoType::Update(const std::vector<std::unique_ptr<Gizmo>> &gizmos)
{
	// Calculates a max instance count over the time of the type. TODO: Allow decreasing max using a timer and average count over the delay.
	//uint32_t instances = INSTANCE_STEPS * static_cast<uint32_t>(std::ceil(static_cast<float>(gizmos.size()) / static_cast<float>(INSTANCE_STEPS)));
	//m_maxInstances = std::max(m_maxInstances, instances);
	m_maxInstances = MAX_INSTANCES;
	m_instances = 0;

	if (gizmos.empty())
	{
		return;
	}

	Instance *instances;
	m_instanceBuffer.MapMemory(reinterpret_cast<void **>(&instances));

	for (const auto &gizmo : gizmos)
	{
		if (m_instances >= m_maxInstances)
		{
			break;
		}

		//if (!Scenes::Get()->GetCamera()->GetViewFrustum().SphereInFrustum(gizmo->GetTransform().GetPosition(), FRUSTUM_BUFFER * gizmo->GetTransform().GetPosition().GetScale()))
		//{
		//continue;
		//}

		auto instance{&instances[m_instances]};
		instance->m_modelMatrix = gizmo->m_transform.GetWorldMatrix();
		instance->m_colour = gizmo->m_colour;
		m_instances++;
	}

	m_instanceBuffer.UnmapMemory();
}

bool GizmoType::CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline, UniformHandler &uniformScene)
{
	if (m_instances == 0)
	{
		return false;
	}

	// Updates descriptors.
	m_descriptorSet.Push("UniformScene", uniformScene);

	if (!m_descriptorSet.Update(pipeline))
	{
		return false;
	}

	vkCmdSetLineWidth(commandBuffer, m_lineThickness);

	// Draws the instanced objects.
	m_descriptorSet.BindDescriptor(commandBuffer, pipeline);

	VkBuffer vertexBuffers[]{ m_model->GetVertexBuffer()->GetBuffer(), m_instanceBuffer.GetBuffer() };
	VkDeviceSize offsets[]{ 0, 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 2, vertexBuffers, offsets);
	vkCmdBindIndexBuffer(commandBuffer, m_model->GetIndexBuffer()->GetBuffer(), 0, m_model->GetIndexType());
	vkCmdDrawIndexed(commandBuffer, m_model->GetIndexCount(), m_instances, 0, 0, 0);
	return true;
}

const Node &operator>>(const Node &node, GizmoType &gizmoType)
{
	node["model"].Get(gizmoType.m_model);
	node["lineThickness"].Get(gizmoType.m_lineThickness);
	node["colour"].Get(gizmoType.m_colour);
	return node;
}

Node &operator<<(Node &node, const GizmoType &gizmoType)
{
	node["model"].Set(gizmoType.m_model);
	node["lineThickness"].Set(gizmoType.m_lineThickness);
	node["colour"].Set(gizmoType.m_colour);
	return node;

}
}
