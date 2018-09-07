#include "PrefabObject.hpp"

#include "Files/Json/FileJson.hpp"
#include "Files/Xml/FileXml.hpp"
#include "Helpers/FileSystem.hpp"
#include "Objects/GameObject.hpp"
#include "Resources/Resources.hpp"
#include "Scenes/Scenes.hpp"

namespace acid
{
	std::shared_ptr<PrefabObject> PrefabObject::Resource(const std::string &filename)
	{
		std::string realFilename = Files::SearchFile(filename);
		auto resource = Resources::Get()->Get(realFilename);

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<PrefabObject>(resource);
		}

		auto result = std::make_shared<PrefabObject>(realFilename);
		Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
		return result;
	}

	PrefabObject::PrefabObject(const std::string &filename) :
		IResource(),
		m_filename(filename),
		m_file(nullptr),
		m_parent(nullptr)
	{
		if (!FileSystem::FileExists(filename))
		{
			FileSystem::CreateFile(filename);
		}

		if (FileSystem::FindExt(filename) == "json")
		{
			m_file = std::make_unique<FileJson>(filename);
			m_file->Load();
			m_parent = m_file->GetParent();
		}
		else if (FileSystem::FindExt(filename) == "xml")
		{
			m_file = std::make_unique<FileXml>(filename);
			m_file->Load();
			m_parent = m_file->GetParent()->FindChild("GameObjectDefinition");
		}
	}

	PrefabObject::~PrefabObject()
	{
	}

	void PrefabObject::Write(const GameObject &gameObject)
	{
		m_file->Clear();

		for (auto &component : gameObject.GetComponents())
		{
			auto componentName = Scenes::Get()->FindComponentName(component.get());

			if (!componentName)
			{
				return;
			}

			auto child = m_parent->FindChild(*componentName);

			if (child == nullptr)
			{
				child = std::make_shared<Metadata>(*componentName);
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
