#include "GizmoType.hpp"

#include "Resources/Resources.hpp"
#include "Scenes/Scenes.hpp"
#include "Gizmo.hpp"

namespace acid
{
static const uint32_t MAX_INSTANCES = 512;
//static const uint32_t INSTANCE_STEPS = 128;
//static const float FRUSTUM_BUFFER = 1.4f;

std::shared_ptr<GizmoType> GizmoType::Create(const Metadata &metadata)
{
	auto resource = Resources::Get()->Find(metadata);

	if (resource != nullptr)
	{
		return std::dynamic_pointer_cast<GizmoType>(resource);
	}

	auto result = std::make_shared<GizmoType>(nullptr);
	Resources::Get()->Add(metadata, std::dynamic_pointer_cast<Resource>(result));
	result->Decode(metadata);
	result->Load();
	return result;
}

std::shared_ptr<GizmoType> GizmoType::Create(const std::shared_ptr<Model> &model, const float &lineThickness, const Colour &colour)
{
	auto temp = GizmoType(model, lineThickness, colour);
	Metadata metadata = Metadata();
	temp.Encode(metadata);
	return Create(metadata);
}

GizmoType::GizmoType(std::shared_ptr<Model> model, const float &lineThickness, const Colour &colour) :
	m_model(std::move(model)),
	m_lineThickness(lineThickness),
	m_colour(colour),
	m_maxInstances(0),
	m_instances(0),
	m_instanceBuffer(sizeof(GizmoTypeData) * MAX_INSTANCES)
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

	GizmoTypeData *gizmoInstances;
	m_instanceBuffer.Map(reinterpret_cast<void **>(&gizmoInstances));

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

		auto instance = &gizmoInstances[m_instances];
		instance->modelMatrix = gizmo->GetTransform().GetWorldMatrix();
		instance->colour = gizmo->GetColour();
		m_instances++;
	}

	m_instanceBuffer.Unmap();
}

bool GizmoType::CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline, UniformHandler &uniformScene)
{
	if (m_instances == 0)
	{
		return false;
	}

	// Updates descriptors.
	m_descriptorSet.Push("UboScene", uniformScene);
	bool updateSuccess = m_descriptorSet.Update(pipeline);

	if (!updateSuccess)
	{
		return false;
	}

	vkCmdSetLineWidth(commandBuffer.GetCommandBuffer(), m_lineThickness);

	// Draws the instanced objects.
	m_descriptorSet.BindDescriptor(commandBuffer, pipeline);

	VkBuffer vertexBuffers[] = { m_model->GetVertexBuffer()->GetBuffer(), m_instanceBuffer.GetBuffer() };
	VkDeviceSize offsets[] = { 0, 0 };
	vkCmdBindVertexBuffers(commandBuffer.GetCommandBuffer(), 0, 2, vertexBuffers, offsets);
	vkCmdBindIndexBuffer(commandBuffer.GetCommandBuffer(), m_model->GetIndexBuffer()->GetBuffer(), 0, m_model->GetIndexType());
	vkCmdDrawIndexed(commandBuffer.GetCommandBuffer(), m_model->GetIndexCount(), m_instances, 0, 0, 0);
	return true;
}

void GizmoType::Decode(const Metadata &metadata)
{
	metadata.GetResource("Model", m_model);
	metadata.GetChild("Line Thickness", m_lineThickness);
	metadata.GetChild("Colour", m_colour);
}

void GizmoType::Encode(Metadata &metadata) const
{
	metadata.SetResource("Model", m_model);
	metadata.SetChild("Line Thickness", m_lineThickness);
	metadata.SetChild("Colour", m_colour);
}

Shader::VertexInput GizmoType::GetVertexInput(const uint32_t &binding)
{
	std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);

	// The vertex input description.
	bindingDescriptions[0].binding = binding;
	bindingDescriptions[0].stride = sizeof(GizmoTypeData);
	bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;

	std::vector<VkVertexInputAttributeDescription> attributeDescriptions(5);

	// Model matrix row 0 attribute.
	attributeDescriptions[0].binding = binding;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32B32A32_SFLOAT;
	attributeDescriptions[0].offset = offsetof(GizmoTypeData, modelMatrix) + offsetof(Matrix4, m_rows[0]);

	// Model matrix row 1 attribute.
	attributeDescriptions[1].binding = binding;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(GizmoTypeData, modelMatrix) + offsetof(Matrix4, m_rows[1]);

	// Model matrix row 2 attribute.
	attributeDescriptions[2].binding = binding;
	attributeDescriptions[2].location = 2;
	attributeDescriptions[2].format = VK_FORMAT_R32G32B32A32_SFLOAT;
	attributeDescriptions[2].offset = offsetof(GizmoTypeData, modelMatrix) + offsetof(Matrix4, m_rows[2]);

	// Model matrix row 3 attribute.
	attributeDescriptions[3].binding = binding;
	attributeDescriptions[3].location = 3;
	attributeDescriptions[3].format = VK_FORMAT_R32G32B32A32_SFLOAT;
	attributeDescriptions[3].offset = offsetof(GizmoTypeData, modelMatrix) + offsetof(Matrix4, m_rows[3]);

	// Colour attribute.
	attributeDescriptions[4].binding = binding;
	attributeDescriptions[4].location = 4;
	attributeDescriptions[4].format = VK_FORMAT_R32G32B32A32_SFLOAT;
	attributeDescriptions[4].offset = offsetof(GizmoTypeData, colour);

	return Shader::VertexInput(binding, bindingDescriptions, attributeDescriptions);
}
}
