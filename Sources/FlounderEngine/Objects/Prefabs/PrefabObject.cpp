#include "PrefabObject.hpp"

#include "../GameObject.hpp"
#include "../../Helpers/FileSystem.hpp"

namespace Flounder
{
	PrefabObject::PrefabObject(const std::string &filename) :
		IResource(),
		m_filename(filename),
		m_fileJson(new FileJson(filename))
	{
		if (!FileSystem::FileExists(filename))
		{
			FileSystem::CreateFile(filename);
		}

		m_fileJson->Load();
	}

	PrefabObject::~PrefabObject()
	{
		delete m_fileJson;
	}

	void PrefabObject::Write(GameObject *gameObject)
	{
		for (auto child : m_fileJson->GetParent()->m_children)
		{
			delete child;
		}

		m_fileJson->GetParent()->m_children.clear();

		for (auto component : *gameObject->GetComponents())
		{
			component->Write(m_fileJson->GetParent()->GetChild(component->GetName(), true));
		}
	}

	void PrefabObject::Save()
	{
		m_fileJson->Save();
	}
}
