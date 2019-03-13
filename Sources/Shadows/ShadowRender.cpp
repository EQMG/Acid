#include "ShadowRender.hpp"

#include "Meshes/Mesh.hpp"
#include "Scenes/Entity.hpp"

namespace acid
{
ShadowRender::ShadowRender() {}

void ShadowRender::Start() {}

void ShadowRender::Update()
{
	// Updates
	// uniforms.
	m_uniformObject.Push("transform", GetParent()->GetWorldMatrix());
}

void ShadowRender::Decode(const Metadata& metadata) {}

void ShadowRender::Encode(Metadata& metadata) const {}

bool ShadowRender::CmdRender(const CommandBuffer& commandBuffer, const PipelineGraphics& pipeline, UniformHandler& uniformScene)
{
	// Gets
	// required
	// components.
	auto mesh = GetParent()->GetComponent<Mesh>();

	if(mesh == nullptr || mesh->GetModel() == nullptr)
		{
			return false;
		}

	// Updates
	// descriptors.
	m_descriptorSet.Push("UboScene", uniformScene);
	m_descriptorSet.Push("UboObject", m_uniformObject);
	bool updateSuccess = m_descriptorSet.Update(pipeline);

	if(!updateSuccess)
		{
			return false;
		}

	// Draws
	// the
	// object.
	m_descriptorSet.BindDescriptor(commandBuffer, pipeline);
	return mesh->GetModel()->CmdRender(commandBuffer);
}
}
