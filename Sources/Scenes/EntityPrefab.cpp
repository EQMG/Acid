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
		result->Decode(metadata);
		result->Load();
		return result;
	}

	std::shared_ptr<EntityPrefab> EntityPrefab::Create(const std::string &filename)
	{
		auto temp = EntityPrefab(filename);
		Metadata metadata = Metadata();
		temp.Encode(metadata);
		return Create(metadata);
	}

	EntityPrefab::EntityPrefab(const std::string &filename) :
		m_filename(filename),
		m_file(nullptr)
	{
		Load();
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

	void EntityPrefab::Decode(const Metadata &metadata)
	{
		metadata.GetChild("Filename", m_filename);
	}

	void EntityPrefab::Encode(Metadata &metadata) const
	{
		metadata.SetChild("Filename", m_filename);
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

			auto child = new Metadata(*componentName);
			m_file->GetMetadata()->AddChild(child);
			component->Encode(*child);
		}
	}

	void EntityPrefab::Save()
	{
		m_file->Write();
	}
}
