#include "ShadowRender.hpp"

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
	// Update push constants.
	m_pushObject.Push("mvp", Shadows::Get()->GetShadowBox().GetProjectionViewMatrix() * GetParent()->GetWorldMatrix());

	// Gets required components.
	auto mesh{GetParent()->GetComponent<Mesh>()};

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

const Metadata &operator>>(const Metadata &metadata, ShadowRender &shadowRender)
{
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const ShadowRender &shadowRender)
{
	return metadata;
}
}
