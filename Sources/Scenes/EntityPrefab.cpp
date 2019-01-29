#include "EntityPrefab.hpp"

#include "Files/File.hpp"
#include "Serialized/Json/Json.hpp"
#include "Serialized/Xml/Xml.hpp"
#include "Helpers/FileSystem.hpp"
#include "Resources/Resources.hpp"
#include "Entity.hpp"
#include "Scenes.hpp"

namespace acid
{
	std::shared_ptr<EntityPrefab> EntityPrefab::Create(const std::string &filename)
	{
		if (filename.empty())
		{
			return nullptr;
		}

		auto resource = Resources::Get()->Find(filename);

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<EntityPrefab>(resource);
		}

		auto result = std::make_shared<EntityPrefab>(filename);
		Resources::Get()->Add(std::dynamic_pointer_cast<Resource>(result));
		return result;
	}

	EntityPrefab::EntityPrefab(const std::string &filename) :
		Resource(filename),
		m_filename(filename),
		m_file(nullptr)
	{
		std::string fileExt = String::Lowercase(FileSystem::FileSuffix(filename));

		if (fileExt == ".json")
		{
			m_file = std::make_unique<File>(filename, new Json());
		}
		else if (fileExt == ".xml")
		{
			m_file = std::make_unique<File>(filename, new Xml("EntityDefinition"));
		}

		m_file->Read();
	}

	void EntityPrefab::Encode(Metadata &metadata) const
	{
		metadata.SetChild<std::string>("Filename", m_filename);
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
