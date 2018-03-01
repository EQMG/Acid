#include "PrefabObject.hpp"

#include "../GameObject.hpp"
#include "../../Helpers/FileSystem.hpp"

namespace Flounder
{
	PrefabObject::PrefabObject(const std::string &filename) :
		IResource(),
		m_filename(filename),
		m_fileJson(new FileJson(filename)),
		m_parent(nullptr)
	{
		if (!FileSystem::FileExists(filename))
		{
			FileSystem::CreateFile(filename);
		}

		m_fileJson->Load();
		m_parent = m_fileJson->GetParent();
	}

	PrefabObject::~PrefabObject()
	{
		delete m_fileJson;
	}

	void PrefabObject::Write(GameObject *gameObject)
	{
		m_parent->m_children.clear();

		for (auto component : *gameObject->GetComponents())
		{
			component->Write(m_parent->GetChild(component->GetName(), true));
		}
	}

	void PrefabObject::Save()
	{
		m_fileJson->Save();
	}
}
