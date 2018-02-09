#include "ComponentSway.hpp"

#include "../../Maths/Maths.hpp"
#include "../Entity.hpp"

namespace Flounder
{
	ComponentSway::ComponentSway(Texture *swayMap) :
		IComponent(),
		m_textureSwayMap(swayMap)
	{
	}

	ComponentSway::ComponentSway(ComponentPrefab *prefab) :
		IComponent(),
		m_textureSwayMap(Texture::Resource(prefab->GetString(0)))
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
		entityRender->descriptorWrites.push_back(m_textureSwayMap->GetWriteDescriptor(5, entityRender->descriptorSet));

		entityRender->uboObject.samples.m_w = 1.0f;
		const float swayPower = 0.15f;
		const float wx = std::sin(GetEntity()->GetTransform()->GetPosition()->m_x * 0.6f);
		const float wz = std::sin(GetEntity()->GetTransform()->GetPosition()->m_z * 0.5f);
		const float sx = std::fabs(wx) + std::fabs(wz);
		const float st = Engine::Get()->GetTime() * sx;
		const float swayX = swayPower * (std::sin(0.25f * st) - std::sin(1.2f * st) + std::cos(0.5f * st));
		const float swayY = swayPower * (std::cos(0.25f * st) - std::cos(1.2f * st) + std::sin(0.5f * st));
		entityRender->uboObject.swaying = Vector2(swayX, swayY);
	}

	void ComponentSway::Save(ComponentPrefab *prefab)
	{
		prefab->SetString(0, m_textureSwayMap->GetFilename());
	}
}
