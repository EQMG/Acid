#include "EntityPrefab.hpp"

#include "Files/File.hpp"
#include "Serialized/Json/Json.hpp"
//#include "Serialized/Xml/Xml.hpp"
//#include "Serialized/Yaml/Yaml.hpp"
#include "Resources/Resources.hpp"
#include "Entity.hpp"
#include "Scenes.hpp"

namespace acid
{
std::shared_ptr<EntityPrefab> EntityPrefab::Create(const Node &node)
{
	auto resource{Resources::Get()->Find(node)};

	if (resource != nullptr)
	{
		return std::dynamic_pointer_cast<EntityPrefab>(resource);
	}

	auto result{std::make_shared<EntityPrefab>("")};
	Resources::Get()->Add(node, std::dynamic_pointer_cast<Resource>(result));
	node >> *result;
	result->Load();
	return result;
}

std::shared_ptr<EntityPrefab> EntityPrefab::Create(const std::filesystem::path &filename)
{
	EntityPrefab temp{filename, false};
	Node node;
	node << temp;
	return Create(node);
}

EntityPrefab::EntityPrefab(std::filesystem::path filename, const bool &load) :
	m_filename{std::move(filename)}
{
	if (load)
	{
		Load();
	}
}

void EntityPrefab::Load()
{
	if (m_filename.empty())
	{
		return;
	}

	auto fileExt{m_filename.extension()};

	if (fileExt == ".json")
	{
		m_file = std::make_unique<File>(m_filename, std::make_unique<Json>());
	}
	/*else if (fileExt == ".yaml")
	{
		m_file = std::make_unique<File>(m_filename, std::make_unique<Yaml>());
	}
	else if (fileExt == ".xml")
	{
		m_file = std::make_unique<File>(m_filename, std::make_unique<Xml>("EntityDefinition"));
	}*/

	if (m_file != nullptr)
	{
		m_file->Load();
	}
}

void EntityPrefab::Write() const
{
	m_file->Write();
}

const EntityPrefab &operator>>(const EntityPrefab &entityPrefab, Entity &entity)
{
	for (const auto &[propertyName, property]: entityPrefab.GetParent()->GetProperties())
	{
		if (propertyName.empty())
		{
			continue;
		}

		auto component{Scenes::Get()->GetComponentRegister().Create(propertyName)};

		if (component == nullptr)
		{
			continue;
		}

		Scenes::Get()->GetComponentRegister().Decode(propertyName, property, component);
		entity.AddComponent(component);
	}

	return entityPrefab;
}

EntityPrefab &operator<<(EntityPrefab &entityPrefab, const Entity &entity)
{
	entityPrefab.m_file->Clear();

	for (const auto &component : entity.GetComponents())
	{
		auto componentName{Scenes::Get()->GetComponentRegister().FindName(component.get())};

		if (!componentName)
		{
			continue;
		}

		auto &property{entityPrefab.m_file->GetNode()->AddProperty(*componentName, {})};
		Scenes::Get()->GetComponentRegister().Encode(*componentName, property, component.get());
	}

	return entityPrefab;
}

const Node &operator>>(const Node &node, EntityPrefab &entityPrefab)
{
	node["filename"].Get(entityPrefab.m_filename);
	return node;
}

Node &operator<<(Node &node, const EntityPrefab &entityPrefab)
{
	node["filename"].Set(entityPrefab.m_filename);
	return node;
}
}
