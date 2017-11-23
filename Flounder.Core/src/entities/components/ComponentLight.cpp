#include "ComponentLight.hpp"

#include "../../engine/Engine.hpp"
#include "../Entity.hpp"

namespace Flounder
{
	ComponentLight::ComponentLight(const Light &light) :
		IComponent(),
		m_light(new Light(light))
	{
	}

	ComponentLight::~ComponentLight()
	{
		delete m_light;
	}

	void ComponentLight::Update()
	{
		m_light->m_position->Set(*GetEntity()->GetTransform()->m_position);
		// TODO: Light-position offset.
	}

	void ComponentLight::CmdRender(EntityRender *entityRender)
	{
	}
}
