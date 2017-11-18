#include "Entity.hpp"
#include "../devices/Display.hpp"

namespace Flounder
{
	Entity::Entity(ISpatialStructure<Entity*> *structure, const Transform &transform) :
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

		for (auto c : *m_components)
		{
			c->CmdRender(&entityRender);
		}

		if (entityRender.model == nullptr)
		{
			return;
		}

		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(entityRender.descriptorWrites.size()), entityRender.descriptorWrites.data(), 0, nullptr);

		VkDescriptorSet descriptors[] = { pipeline.GetDescriptorSet() };
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
		return nullptr;
	}

	void Entity::Remove()
	{
		m_structure->Remove(this);
		m_removed = true;
		m_structure = nullptr;
	}
}
