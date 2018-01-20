#include "ComponentMaterial.hpp"

#include "../Entity.hpp"

namespace Flounder
{
	ComponentMaterial::ComponentMaterial(const float &metallic, const float &roughness, const bool &castsShadows) :
		IComponent(),
		m_metallic(metallic),
		m_roughness(roughness),
		m_castsShadows(castsShadows)
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
	}
}
