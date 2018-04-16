#include "VoxelRender.hpp"

#include "Meshes/Mesh.hpp"
#include "Worlds/Worlds.hpp"
#include "UbosVoxels.hpp"

namespace Flounder
{
	VoxelRender::VoxelRender() :
		Component(),
		m_uniformObject(new UniformBuffer(sizeof(UbosVoxels::UboObject))),
		m_descriptorSet(nullptr)
	{
	}

	VoxelRender::~VoxelRender()
	{
		delete m_uniformObject;
		delete m_descriptorSet;
	}

	void VoxelRender::Update()
	{
		// Updates uniforms.
		UbosVoxels::UboObject uboObject = {};
		GetGameObject()->GetTransform()->GetWorldMatrix(&uboObject.transform);
		m_uniformObject->Update(&uboObject);
	}

	void VoxelRender::Load(LoadedValue *value)
	{
	}

	void VoxelRender::Write(LoadedValue *value)
	{
	}

	void VoxelRender::CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, UniformBuffer *uniformScene)
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
