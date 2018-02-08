#include "ComponentGlow.hpp"

#include "../Entity.hpp"

namespace Flounder
{
	ComponentGlow::ComponentGlow(Texture *swayMap) :
		IComponent(),
		m_textureGlowMap(swayMap)
	{
	}

	ComponentGlow::ComponentGlow(ComponentPrefab *prefab) :
		IComponent(),
		m_textureGlowMap(Texture::Resource(prefab->GetString(0)))
	{
	}

	ComponentGlow::~ComponentGlow()
	{
	}

	void ComponentGlow::Update()
	{
	}

	void ComponentGlow::CmdRender(EntityRender *entityRender)
	{
	// 	entityRender->descriptorWrites.push_back(m_textureGlowMap->GetWriteDescriptor(5, entityRender->descriptorSet));
	}

	void ComponentGlow::Save(ComponentPrefab *prefab)
	{
		prefab->SetString(0, m_textureGlowMap->GetFilename());
	}
}
