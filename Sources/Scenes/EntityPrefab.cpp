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
	auto temp = EntityPrefab(filename, false);
	Metadata metadata = Metadata();
	metadata << temp;
	return Create(metadata);
}

EntityPrefab::EntityPrefab(std::string filename, const bool &load) :
	m_filename(std::move(filename))
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

	std::string fileExt = String::Lowercase(FileSystem::FileSuffix(m_filename));

	if (fileExt == ".json")
	{
		m_file = std::make_unique<File>(m_filename, new Json());
	}
	else if (fileExt == ".yaml")
	{
		m_file = std::make_unique<File>(m_filename, new Yaml());
	}
	else if (fileExt == ".xml")
	{
		m_file = std::make_unique<File>(m_filename, new Xml("EntityDefinition"));
	}

	if (m_file != nullptr)
	{
		m_file->Read();
	}
}

void EntityPrefab::Write(const Entity &entity)
{
	m_file->GetMetadata()->ClearChildren();

	for (const auto &component : entity.GetComponents())
	{
		auto componentName = Scenes::Get()->GetComponentRegister().FindName(component.get());

		if (!componentName)
		{
			continue;
		}

		auto child = m_file->GetMetadata()->AddChild(new Metadata(*componentName));
		Scenes::Get()->GetComponentRegister().Encode(*componentName, *child, component.get());
	}
}

void EntityPrefab::Save()
{
	m_file->Write();
}

const Metadata &operator>>(const Metadata &metadata, EntityPrefab &enityPrefab)
{
	metadata.GetChild("Filename", enityPrefab.m_filename);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const EntityPrefab &enityPrefab)
{
	metadata.SetChild("Filename", enityPrefab.m_filename);
	return metadata;
}
}
