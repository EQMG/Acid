#include "Entity.hpp"

#include "../Devices/Display.hpp"

namespace Flounder
{
	Entity::Entity(ISpatialStructure<Entity*> *structure, const Transform &transform) :
		m_uniformObject(new UniformBuffer(sizeof(UbosEntities::UboObject))),
		m_structure(structure),
		m_components(new std::vector<IComponent*>()),
		m_transform(new Transform(transform)),
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

		for (auto c : *m_components)
		{
			delete c;
		}

		delete m_components;
		delete m_transform;
		delete m_uniformObject;
	}

	void Entity::Update()
	{
		for (auto c : *m_components)
		{
			c->Update();
		}
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

		for (auto c : *m_components)
		{
			c->CmdRender(&entityRender);
		}

		if (entityRender.model == nullptr)
		{
			return;
		}

		m_uniformObject->Update(&entityRender.uboObject);

		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(entityRender.descriptorWrites.size()), entityRender.descriptorWrites.data(), 0, nullptr);

		VkDescriptorSet descriptors[] = { descriptorSet };
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);

		entityRender.model->CmdRender(commandBuffer);
	}

	void Entity::MoveStructure(ISpatialStructure<Entity*> *structure)
	{
		m_structure->Remove(this);
		structure->Add(this);
		m_structure = structure;
	}

	void Entity::AddComponent(IComponent *component)
	{
		m_components->push_back(component);
		component->SetEntity(this);
	}

	void Entity::RemoveComponent(IComponent *component)
	{
		for (auto it = m_components->begin(); it != m_components->end(); ++it)
		{
			if (*it == component)
			{
				component->SetEntity(nullptr);
				delete component;
				m_components->erase(it);
				return;
			}
		}
	}

	ICollider *Entity::GetCollider()
	{
		for (auto c : *m_components)
		{
			if (c->GetCollider() != nullptr)
			{
				return c->GetCollider();
			}
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
