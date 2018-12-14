#include "PrefabObject.hpp"

#include "Files/Json/FileJson.hpp"
#include "Files/Xml/FileXml.hpp"
#include "Helpers/FileSystem.hpp"
#include "Objects/Entity.hpp"
#include "Resources/Resources.hpp"
#include "Scenes/Scenes.hpp"

namespace acid
{
	std::shared_ptr<PrefabObject> PrefabObject::Resource(const std::string &filename)
	{
		if (filename.empty())
		{
			return nullptr;
		}

		auto resource = Resources::Get()->Get(filename);

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<PrefabObject>(resource);
		}

		auto result = std::make_shared<PrefabObject>(filename);
		Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
		return result;
	}

	PrefabObject::PrefabObject(const std::string &filename) :
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

	void PrefabObject::Write(const Entity &entity)
	{
		m_parent->ClearChildren();

		for (auto &component : entity.GetComponents())
		{
			auto componentName = Scenes::Get()->FindComponentName(component.get());

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

	void PrefabObject::Save()
	{
		m_file->Save();
	}
}
