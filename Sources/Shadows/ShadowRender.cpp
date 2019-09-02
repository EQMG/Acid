#include "ShadowRender.hpp"

#include "Maths/Transform.hpp"
#include "Meshes/Mesh.hpp"
#include "Scenes/Entity.hpp"
#include "Shadows.hpp"

namespace acid
{
ShadowRender::ShadowRender()
{
}

void ShadowRender::Start()
{
}

void ShadowRender::Update()
{
}

bool ShadowRender::CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline)
{
	auto transform = GetEntity()->GetComponent<Transform>();

	if (transform == nullptr)
	{
		return false;
	}

	// Update push constants.
	m_pushObject.Push("mvp", Shadows::Get()->GetShadowBox().GetProjectionViewMatrix() * transform->GetWorldMatrix());

	// Gets required components.
	auto mesh = GetEntity()->GetComponent<Mesh>();

	if (mesh == nullptr || mesh->GetModel() == nullptr)
	{
		return false;
	}

	// Updates descriptors.
	m_descriptorSet.Push("PushObject", m_pushObject);

	if (!m_descriptorSet.Update(pipeline))
	{
		return false;
	}

	// Draws the object.
	m_descriptorSet.BindDescriptor(commandBuffer, pipeline);
	m_pushObject.BindPush(commandBuffer, pipeline);
	return mesh->GetModel()->CmdRender(commandBuffer);
}

const Node &operator>>(const Node &node, ShadowRender &shadowRender)
{
	return node;
}

Node &operator<<(Node &node, const ShadowRender &shadowRender)
{
	return node;
}
}
