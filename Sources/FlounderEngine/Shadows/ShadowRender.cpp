#include "ShadowRender.hpp"

#include "Meshes/Mesh.hpp"
#include "Worlds/Worlds.hpp"
#include "UbosShadows.hpp"

namespace Flounder
{
	ShadowRender::ShadowRender() :
		Component(),
		m_uniformObject(new UniformBuffer(sizeof(UbosShadows::UboObject))),
		m_descriptorSet(nullptr)
	{
	}

	ShadowRender::~ShadowRender()
	{
		delete m_uniformObject;
		delete m_descriptorSet;
	}

	void ShadowRender::Update()
	{
		// Updates uniforms.
		UbosShadows::UboObject uboObject = {};
		GetGameObject()->GetTransform()->GetWorldMatrix(&uboObject.transform);
		m_uniformObject->Update(&uboObject);
	}

	void ShadowRender::Load(LoadedValue *value)
	{
	}

	void ShadowRender::Write(LoadedValue *value)
	{
	}

	void ShadowRender::CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, UniformBuffer *uniformScene)
	{
		// Gets required components.
		auto mesh = GetGameObject()->GetComponent<Mesh>();

		if (mesh == nullptr || mesh->GetModel() == nullptr)
		{
			return;
		}

		// Updates descriptors.
		if (m_descriptorSet == nullptr)
		{
			m_descriptorSet = new DescriptorSet(pipeline);
		}

		m_descriptorSet->Update({
			uniformScene,
			m_uniformObject
		});

		// Draws the object.
		m_descriptorSet->BindDescriptor(commandBuffer);
		mesh->GetModel()->CmdRender(commandBuffer);
	}
}
