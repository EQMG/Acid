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

	ComponentMaterial::ComponentMaterial(ComponentPrefab* prefab) :
		IComponent(),
		m_metallic(atof(prefab->m_data.at(0).c_str())),
		m_roughness(atof(prefab->m_data.at(1).c_str())),
		m_castsShadows(atoi(prefab->m_data.at(2).c_str()) == 1),
		m_ignoreLighting(atoi(prefab->m_data.at(3).c_str()) == 1),
		m_ignoreFog(atoi(prefab->m_data.at(4).c_str()) == 1)
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
		entityRender->uboObject.ignore = (1.0f / 3.0f) * ((float)m_ignoreFog + (2.0f * (float)m_ignoreLighting));
	}
}
