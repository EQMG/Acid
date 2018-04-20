#include "ShadowRender.hpp"

#include "Meshes/Mesh.hpp"

namespace Flounder
{
	ShadowRender::ShadowRender() :
		Component(),
		m_descriptorSet(new DescriptorsHandler()),
		m_uniformObject(new UniformHandler())
	{
	}

	ShadowRender::~ShadowRender()
	{
		delete m_descriptorSet;
		delete m_uniformObject;
	}

	void ShadowRender::Update()
	{
		// Updates uniforms.
		m_uniformObject->Push("transform", GetGameObject()->GetTransform()->GetWorldMatrix());
	}

	void ShadowRender::Load(LoadedValue *value)
	{
	}

	void ShadowRender::Write(LoadedValue *value)
	{
	}

	void ShadowRender::CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, UniformHandler *uniformScene)
	{
		// Gets required components.
		auto mesh = GetGameObject()->GetComponent<Mesh>();

		if (mesh == nullptr || mesh->GetModel() == nullptr)
		{
			return;
		}

		// Updates descriptors.
		m_descriptorSet->Push("UboScene", uniformScene);
		m_descriptorSet->Push("UboObject", m_uniformObject);
		bool descriptorsSet = m_descriptorSet->Update(pipeline);

		if (!descriptorsSet)
		{
			return;
		}

		// Draws the object.
		m_descriptorSet->GetDescriptorSet()->BindDescriptor(commandBuffer);
		mesh->GetModel()->CmdRender(commandBuffer);
	}
}
