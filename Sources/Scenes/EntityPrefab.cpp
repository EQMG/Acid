#include "EntityPrefab.hpp"

#include "Files/File.hpp"
#include "Serialized/Json/Json.hpp"
#include "Serialized/Xml/Xml.hpp"
#include "Serialized/Yaml/Yaml.hpp"
#include "Files/FileSystem.hpp"
#include "Resources/Resources.hpp"
#include "Entity.hpp"
#include "Scenes.hpp"

namespace acid
{
std::shared_ptr<EntityPrefab> EntityPrefab::Create(const Metadata &metadata)
{
	auto resource = Resources::Get()->Find(metadata);

	if (resource != nullptr)
	{
		return std::dynamic_pointer_cast<EntityPrefab>(resource);
	}

	auto result = std::make_shared<EntityPrefab>("");
	Resources::Get()->Add(metadata, std::dynamic_pointer_cast<Resource>(result));
	metadata >> *result;
	result->Load();
	return result;
}

std::shared_ptr<EntityPrefab> EntityPrefab::Create(const std::string &filename)
{
	EntityPrefab temp{filename, false};
	Metadata metadata;
	metadata << temp;
	return Create(metadata);
}

EntityPrefab::EntityPrefab(std::string filename, const bool &load) :
	m_filename{std::move(filename)}
{
	if (load)
	{
		EntityPrefab::Load();
	}
}

void EntityPrefab::Load()
{
	if (m_filename.empty())
	{
		return;
	}

	auto fileExt = String::Lowercase(FileSystem::FileSuffix(m_filename));

	if (fileExt == ".json")
	{
		m_file = std::make_unique<File>(m_filename, std::make_unique<Json>());
	}
	else if (fileExt == ".yaml")
	{
		m_file = std::make_unique<File>(m_filename, std::make_unique<Yaml>());
	}
	else if (fileExt == ".xml")
	{
		m_file = std::make_unique<File>(m_filename, std::make_unique<Xml>("EntityDefinition"));
	}

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
	for (const auto &child : entityPrefab.GetParent()->GetChildren())
	{
		if (child->GetName().empty())
		{
			continue;
		}

		auto component = Scenes::Get()->GetComponentRegister().Create(child->GetName());

		if (component == nullptr)
		{
			continue;
		}

		Scenes::Get()->GetComponentRegister().Decode(child->GetName(), *child, component);
		entity.AddComponent(component);
	}

	return entityPrefab;
}

EntityPrefab &operator<<(EntityPrefab &entityPrefab, const Entity &entity)
{
	entityPrefab.m_file->GetMetadata()->ClearChildren();

	for (const auto &component : entity.GetComponents())
	{
		auto componentName = Scenes::Get()->GetComponentRegister().FindName(component.get());

		if (!componentName)
		{
			continue;
		}

		auto child = entityPrefab.m_file->GetMetadata()->AddChild(new Metadata(*componentName));
		Scenes::Get()->GetComponentRegister().Encode(*componentName, *child, component.get());
	}

	return entityPrefab;
}

const Metadata &operator>>(const Metadata &metadata, EntityPrefab &entityPrefab)
{
	metadata.GetChild("Filename", entityPrefab.m_filename);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const EntityPrefab &entityPrefab)
{
	metadata.SetChild("Filename", entityPrefab.m_filename);
	return metadata;
}
}
