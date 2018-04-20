#include "VoxelRender.hpp"

#include "Meshes/Mesh.hpp"

namespace Flounder
{
	VoxelRender::VoxelRender() :
		Component(),
		m_descriptorSet(new DescriptorsHandler()),
		m_uniformObject(new UniformHandler())
	{
	}

	VoxelRender::~VoxelRender()
	{
		delete m_descriptorSet;
		delete m_uniformObject;
	}

	void VoxelRender::Update()
	{
		// Updates uniforms.
		m_uniformObject->Push("transform", GetGameObject()->GetTransform()->GetWorldMatrix());
	}

	void VoxelRender::Load(LoadedValue *value)
	{
	}

	void VoxelRender::Write(LoadedValue *value)
	{
	}

	void VoxelRender::CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, UniformHandler *uniformScene)
	{
		// Gets required components.
		auto mesh = GetGameObject()->GetComponent<Mesh>();

		if (mesh == nullptr || mesh->GetModel() == nullptr)
		{
			return;
		}

		//auto rigidbody = GetGameObject()->GetComponent<Rigidbody>();

		/*if (rigidbody != nullptr && rigidbody->GetCollider() != nullptr)
		{
			if (!rigidbody->GetCollider()->InFrustum(*Scenes::Get()->GetCamera()->GetViewFrustum()))
			{
				return;
			}
		}*/

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
