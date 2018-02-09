#include "ComponentDiffuse.hpp"

#include "../Entity.hpp"

namespace Flounder
{
	ComponentDiffuse::ComponentDiffuse(Texture *diffuse) :
		IComponent(),
		m_textureDiffuse(diffuse)
	{
	}

	ComponentDiffuse::ComponentDiffuse(ComponentPrefab *prefab) :
		IComponent(),
		m_textureDiffuse(Texture::Resource(prefab->GetString(0)))
	{

	}

	ComponentDiffuse::~ComponentDiffuse()
	{
	}

	void ComponentDiffuse::Update()
	{
	}

	void ComponentDiffuse::CmdRender(EntityRender *entityRender)
	{
		entityRender->descriptorWrites.push_back(m_textureDiffuse->GetWriteDescriptor(2, entityRender->descriptorSet));

		entityRender->uboObject.samples.m_x = 1.0f;
	}

	void ComponentDiffuse::Save(ComponentPrefab *prefab)
	{
		prefab->SetString(0, m_textureDiffuse->GetFilename());
	}
}
