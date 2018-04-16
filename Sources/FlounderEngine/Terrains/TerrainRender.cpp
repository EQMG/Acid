#include "TerrainRender.hpp"

#include "Meshes/Mesh.hpp"
#include "Worlds/Worlds.hpp"
#include "UbosTerrains.hpp"

namespace Flounder
{
	TerrainRender::TerrainRender() :
		Component(),
		m_uniformObject(new UniformBuffer(sizeof(UbosTerrains::UboObject))),
		m_descriptorSet(nullptr)
	{
	}

	TerrainRender::~TerrainRender()
	{
		delete m_uniformObject;
		delete m_descriptorSet;
	}

	void TerrainRender::Update()
	{
		// Updates uniforms.
		UbosTerrains::UboObject uboObject = {};
		GetGameObject()->GetTransform()->GetWorldMatrix(&uboObject.transform);
		m_uniformObject->Update(&uboObject);
	}

	void TerrainRender::Load(LoadedValue *value)
	{
	}

	void TerrainRender::Write(LoadedValue *value)
	{
	}

	void TerrainRender::CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, UniformBuffer *uniformScene)
	{
		// Gets required components.
		auto mesh = GetGameObject()->GetComponent<Mesh>();

		if (mesh == nullptr || mesh->GetModel() == nullptr)
		{
			return;
		}

		/*auto rigidbody = GetGameObject()->GetComponent<Rigidbody>();

		if (rigidbody != nullptr && rigidbody->GetCollider() != nullptr)
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
