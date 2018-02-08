#include "ComponentMaterial.hpp"

#include "../Entity.hpp"

namespace Flounder
{
	ComponentMaterial::ComponentMaterial(const float &metallic, const float &roughness, const bool &castsShadows, const bool &ignoreLighting, const bool &ignoreFog) :
		IComponent(),
		m_metallic(metallic),
		m_roughness(roughness),
		m_castsShadows(castsShadows),
		m_ignoreLighting(ignoreLighting),
		m_ignoreFog(ignoreFog)
	{
	}

	ComponentMaterial::ComponentMaterial(ComponentPrefab *prefab) :
		IComponent(),
		m_metallic(prefab->GetFloat(0)),
		m_roughness(prefab->GetFloat(1)),
		m_castsShadows(prefab->GetBool(2)),
		m_ignoreLighting(prefab->GetBool(3)),
		m_ignoreFog(prefab->GetBool(4))
	{
	}

	ComponentMaterial::~ComponentMaterial()
	{
	}

	void ComponentMaterial::Update()
	{
	}

	void ComponentMaterial::CmdRender(EntityRender *entityRender)
	{
		entityRender->uboObject.metallic = m_metallic;
		entityRender->uboObject.roughness = m_roughness;
		entityRender->uboObject.ignore = (1.0f / 3.0f) * ((float) m_ignoreFog + (2.0f * (float) m_ignoreLighting));
	}

	void ComponentMaterial::Save(ComponentPrefab *prefab)
	{
		prefab->SetFloat(0, m_metallic);
		prefab->SetFloat(1, m_roughness);
		prefab->SetBool(2, m_castsShadows);
		prefab->SetBool(3, m_ignoreLighting);
		prefab->SetBool(4, m_ignoreFog);
	}
}
