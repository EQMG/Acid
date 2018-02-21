#include "ComponentLight.hpp"

#include "../Entity.hpp"

namespace Flounder
{
	ComponentLight::ComponentLight(const Light &light, const Vector3 &offset) :
		Component(),
		m_light(new Light(light)),
		m_offset(new Vector3(offset))
	{
	}

	ComponentLight::ComponentLight(ComponentPrefab *prefab) :
		Component(),
		m_light(new Light(Colour(prefab->GetString(0)), prefab->GetFloat(1))),
		m_offset(new Vector3(prefab->GetFloat(2), prefab->GetFloat(3), prefab->GetFloat(4)))
	{
	}

	ComponentLight::~ComponentLight()
	{
		delete m_light;
		delete m_offset;
	}

	void ComponentLight::Update()
	{
		m_light->m_position->Set(*GetGameObject()->GetTransform()->m_position);
		Vector3::Add(*m_light->m_position, *m_offset, m_light->m_position);
	}

	void ComponentLight::CmdRender(EntityRender *entityRender)
	{
	}

//	void ComponentLight::Save(ComponentPrefab *prefab)
//	{
//		prefab->SetString(0, Colour::GetHex(*m_light->m_colour));
//		prefab->SetFloat(1, m_light->m_radius);
//		prefab->SetFloat(2, m_offset->m_x);
//		prefab->SetFloat(3, m_offset->m_y);
//		prefab->SetFloat(4, m_offset->m_z);
//	}
}
