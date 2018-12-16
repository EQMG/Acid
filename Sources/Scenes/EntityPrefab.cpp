#include "EntityPrefab.hpp"

#include "Files/Json/FileJson.hpp"
#include "Files/Xml/FileXml.hpp"
#include "Helpers/FileSystem.hpp"
#include "Resources/Resources.hpp"
#include "Entity.hpp"
#include "Scenes.hpp"

namespace acid
{
	std::shared_ptr<EntityPrefab> EntityPrefab::Resource(const std::string &filename)
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
		Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
		return result;
	}

	EntityPrefab::EntityPrefab(const std::string &filename) :
		IResource(),
		m_filename(filename),
		m_file(nullptr),
		m_parent(nullptr)
	{
		std::string fileExt = String::Lowercase(FileSystem::FileSuffix(filename));

		if (fileExt == ".json")
		{
			m_file = std::make_unique<FileJson>(filename);
			m_file->Load();
			m_parent = m_file->GetParent();
		}
		else if (fileExt == ".xml")
		{
			m_file = std::make_unique<FileXml>(filename);
			m_file->Load();
			m_parent = m_file->GetParent()->FindChild("EntityDefinition");

			if (m_parent == nullptr)
			{
				m_parent = m_file->GetParent()->AddChild(new Metadata("EntityDefinition"));
			}
		}
	}

	void EntityPrefab::Write(const Entity &entity)
	{
		m_parent->ClearChildren();

		for (auto &component : entity.GetComponents())
		{
			auto componentName = Scenes::Get()->GetComponentRegister().FindName(component.get());

			if (!componentName)
			{
				continue;
			}

			auto child = m_parent->FindChild(*componentName, false);

			if (child == nullptr)
			{
				child = new Metadata(*componentName);
				m_parent->AddChild(child);
			}

			component->Encode(*child);
		}
	}

	void EntityPrefab::Save()
	{
		m_file->Save();
	}
}
