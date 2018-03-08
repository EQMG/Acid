#include "TerrainRender.hpp"

#include "../Devices/Display.hpp"
#include "../Meshes/Mesh.hpp"
#include "../Physics/Rigidbody.hpp"
#include "../Scenes/Scenes.hpp"
#include "UbosTerrains.hpp"

namespace Flounder
{
	const int TerrainRender::SIDE_LENGTH = 200;
	const std::vector<float> TerrainRender::SQUARE_SIZES = {
		2.0f, 8.0f, 25.0f, 50.0f
	};
	const std::vector<float> TerrainRender::TEXTURE_SCALES = {
		10.0f, 5.0f, 2.0f, 1.0f
	};

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
	}

	void TerrainRender::Load(LoadedValue *value)
	{
	}

	void TerrainRender::Write(LoadedValue *value)
	{
	}

	void TerrainRender::CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, const UniformBuffer &uniformScene)
	{
		if (m_descriptorSet == nullptr)
		{
			m_descriptorSet = new DescriptorSet(pipeline);
			std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>{
				uniformScene.GetWriteDescriptor(0, *m_descriptorSet),
				m_uniformObject->GetWriteDescriptor(1, *m_descriptorSet)
			};
			m_descriptorSet->Update(descriptorWrites);
		}

		// Gets required components.
		auto mesh = GetGameObject()->GetComponent<Mesh>();
		auto rigidbody = GetGameObject()->GetComponent<Rigidbody>();

		if (mesh == nullptr || mesh->GetModel() == nullptr)
		{
			return;
		}

		if (rigidbody != nullptr && rigidbody->GetCollider() != nullptr)
		{
			if (!rigidbody->GetCollider()->InFrustum(*Scenes::Get()->GetCamera()->GetViewFrustum()))
			{
				return;
			}
		}

		// Creates a UBO object and write descriptor.
		UbosTerrains::UboObject uboObject = {};
		GetGameObject()->GetTransform()->GetWorldMatrix(&uboObject.transform);
		m_uniformObject->Update(&uboObject);

		// Draws the object.
		m_descriptorSet->BindDescriptor(commandBuffer, pipeline);
		mesh->GetModel()->CmdRender(commandBuffer);
	}
}
