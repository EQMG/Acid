#include "ShadowRender.hpp"

#include "Meshes/Mesh.hpp"

namespace acid
{
	ShadowRender::ShadowRender() :
		IComponent(),
		m_descriptorSet(DescriptorsHandler()),
		m_uniformObject(UniformHandler())
	{
	}

	ShadowRender::~ShadowRender()
	{
	}

	void ShadowRender::Start()
	{
	}

	void ShadowRender::Update()
	{
		// Updates uniforms.
		m_uniformObject.Push("transform", GetGameObject()->GetTransform().GetWorldMatrix());
	}

	void ShadowRender::Decode(const Serialized &serialized)
	{
	}

	void ShadowRender::Encode(Serialized &serialized) const
	{
	}

	void ShadowRender::CmdRender(const CommandBuffer &commandBuffer, const Pipeline &pipeline, UniformHandler &uniformScene)
	{
		// Gets required components.
		auto mesh = GetGameObject()->GetComponent<Mesh>();

		if (mesh == nullptr || mesh->GetModel() == nullptr)
		{
			return;
		}

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", uniformScene);
		m_descriptorSet.Push("UboObject", m_uniformObject);
		bool updateSuccess = m_descriptorSet.Update(pipeline);

		if (!updateSuccess)
		{
			return;
		}

		// Draws the object.
		m_descriptorSet.BindDescriptor(commandBuffer);
		mesh->GetModel()->CmdRender(commandBuffer);
	}
}
