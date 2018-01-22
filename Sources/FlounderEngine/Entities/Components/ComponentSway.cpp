#include "ComponentSway.hpp"

#include <math.h>
#include "../Entity.hpp"

namespace Flounder
{
	ComponentSway::ComponentSway(Texture *swayMap) :
		IComponent(),
		m_textureSwayMap(swayMap)
	{
	}

	ComponentSway::~ComponentSway()
	{
	}

	void ComponentSway::Update()
	{
	}

	void ComponentSway::CmdRender(EntityRender *entityRender)
	{
		entityRender->descriptorWrites.push_back(m_textureSwayMap->GetWriteDescriptor(3, entityRender->descriptorSet));

		entityRender->uboObject.swaying = 1.0f;
		const float swayPower = 0.15f;
		const float wx = sin(GetEntity()->GetTransform()->GetPosition()->m_x * 0.6f);
		const float wz = sin(GetEntity()->GetTransform()->GetPosition()->m_z * 0.5f);
		const float sx = fabs(wx) + fabs(wz);
		const float st = Engine::Get()->GetTime() * sx;
		const float swayX = swayPower * (sin(0.25f * st) - sin(1.2f * st) + cos(0.5f * st));
		const float swayY = swayPower * (cos(0.25f * st) - cos(1.2f * st) + sin(0.5f * st));
		entityRender->uboObject.swayOffset = Vector2(swayX, swayY);
	}
}
