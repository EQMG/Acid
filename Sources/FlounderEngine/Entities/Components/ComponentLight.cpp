#include "ComponentLight.hpp"

#include "../../Engine/Engine.hpp"
#include "../Entity.hpp"

namespace Flounder
{
	ComponentLight::ComponentLight(const Light &light, const Vector3 &offset) :
		IComponent(),
		m_light(new Light(light)),
		m_offset(new Vector3(offset))
	{
	}

	ComponentLight::~ComponentLight()
	{
		delete m_light;
		delete m_offset;
	}

	void ComponentLight::Update()
	{
		m_light->m_position->Set(*GetEntity()->GetTransform()->m_position);
		Vector3::Add(*m_light->m_position, *m_offset, m_light->m_position);
	}

	void ComponentLight::CmdRender(EntityRender *entityRender)
	{
	}
}
