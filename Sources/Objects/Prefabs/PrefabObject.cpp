#include "PrefabObject.hpp"

#include "Objects/GameObject.hpp"
#include "Helpers/FileSystem.hpp"
#include "Files/Json/FileJson.hpp"
#include "Files/Xml/FileXml.hpp"

namespace acid
{
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
			m_file = std::make_shared<FileJson>(filename);
			m_file->Load();
			m_parent = m_file->GetParent();
		}
		else if (FileSystem::FindExt(filename) == "xml")
		{
			m_file = std::make_shared<FileXml>(filename);
			m_file->Load();
			m_parent = m_file->GetParent()->GetChild("GameObjectDefinition");
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
			component->Write(m_parent->GetChild(component->GetName(), true));
		}
	}

	void PrefabObject::Save()
	{
		m_file->Save();
	}
}
