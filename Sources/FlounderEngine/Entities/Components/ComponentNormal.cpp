#include "ComponentNormal.hpp"

#include "../Entity.hpp"

namespace Flounder
{
	ComponentNormal::ComponentNormal(Texture *normal) :
		IComponent(),
		m_textureNormal(normal)
	{
	}

	ComponentNormal::ComponentNormal(ComponentPrefab *prefab) :
		IComponent(),
		m_textureNormal(Texture::Resource(prefab->GetString(0)))
	{

	}

	ComponentNormal::~ComponentNormal()
	{
	}

	void ComponentNormal::Update()
	{
	}

	void ComponentNormal::CmdRender(EntityRender *entityRender)
	{
	//	entityRender->descriptorWrites.push_back(m_textureSwayMap->GetWriteDescriptor(4, entityRender->descriptorSet));
	}

	void ComponentNormal::Save(ComponentPrefab *prefab)
	{
		prefab->SetString(0, m_textureNormal->GetFilename());
	}
}
