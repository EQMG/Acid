#include "ComponentNormal.hpp"

#include "../Entity.hpp"

namespace Flounder
{
	ComponentNormal::ComponentNormal(Texture *normal) :
		Component(),
		m_textureNormal(normal)
	{
	}

	ComponentNormal::ComponentNormal(ComponentPrefab *prefab) :
		Component(),
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
		entityRender->descriptorWrites.push_back(m_textureNormal->GetWriteDescriptor(3, entityRender->descriptorSet));

		entityRender->uboObject.samples.m_y = 1.0f;
	}

//	void ComponentNormal::Save(ComponentPrefab *prefab)
//	{
//		prefab->SetString(0, m_textureNormal->GetFilename());
//	}
}
