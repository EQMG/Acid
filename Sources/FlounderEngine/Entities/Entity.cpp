#include "Entity.hpp"

#include "../Devices/Display.hpp"
#include "Entities.hpp"
#include "Prefabs/Components.hpp"
#include "Prefabs/EntityPrefab.hpp"

namespace Flounder
{
	Entity::Entity(const std::string &prefabName, const Transform &transform, ISpatialStructure<Entity *> *structure) :
		GameObject(transform),
		m_uniformObject(new UniformBuffer(sizeof(UbosEntities::UboObject))),
		m_structure(structure),
		m_prefabName(prefabName),
		m_removed(false)
	{
		if (m_structure == nullptr)
		{
			m_structure = Entities::Get()->GetStructure();
		}

		if (m_structure != nullptr)
		{
			m_structure->Add(this);
		}

		EntityPrefab *entityPrefab = EntityPrefab::Resource("Resources/Entities/" + prefabName + "/" + prefabName + ".csv");
		Components::AttachAllTo(entityPrefab, this);
	}

	Entity::Entity(const Transform &transform, ISpatialStructure<Entity *> *structure) :
		GameObject(transform),
		m_uniformObject(new UniformBuffer(sizeof(UbosEntities::UboObject))),
		m_structure(structure),
		m_prefabName(""),
		m_removed(false)
	{
		if (m_structure != nullptr)
		{
			m_structure->Add(this);
		}
	}

	Entity::~Entity()
	{
		Remove();

		delete m_uniformObject;
	}

	void Entity::Update()
	{
	}

	void Entity::CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, const UniformBuffer &uniformScene)
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto descriptorSet = pipeline.GetDescriptorSet();

		EntityRender entityRender = {};
		entityRender.descriptorSet = descriptorSet;
		entityRender.uboObject = {};
		entityRender.descriptorWrites = std::vector<VkWriteDescriptorSet>();
		entityRender.model = nullptr;

		m_transform->GetWorldMatrix(&entityRender.uboObject.transform);
		entityRender.descriptorWrites.push_back(uniformScene.GetWriteDescriptor(0, descriptorSet));
		entityRender.descriptorWrites.push_back(m_uniformObject->GetWriteDescriptor(1, descriptorSet));

	//	for (auto c : *m_components)
	//	{
	//		c->CmdRender(&entityRender);
	//	}

		if (entityRender.model == nullptr)
		{
			return;
		}

		m_uniformObject->Update(&entityRender.uboObject);

		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(entityRender.descriptorWrites.size()), entityRender.descriptorWrites.data(), 0, nullptr);

		VkDescriptorSet descriptors[] = {descriptorSet};
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);

		entityRender.model->CmdRender(commandBuffer);
	}

	void Entity::MoveStructure(ISpatialStructure<Entity *> *structure)
	{
		m_structure->Remove(this);
		structure->Add(this);
		m_structure = structure;
	}

	ICollider *Entity::GetCollider()
	{
		auto componentCollider = GetComponent<ComponentCollider>();

		if (componentCollider != nullptr)
		{
			return componentCollider->GetCollider();
		}

		return nullptr;
	}

	void Entity::Remove()
	{
		m_structure->Remove(this);
		m_removed = true;
		m_structure = nullptr;
	}
}
