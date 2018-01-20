#include "ComponentCollider.hpp"

#include "../Entity.hpp"
#include "ComponentModel.hpp"

namespace Flounder
{
	ComponentCollider::ComponentCollider() :
		IComponent(),
		m_collider(nullptr)
	{
	}

	ComponentCollider::~ComponentCollider()
	{
	}

	void ComponentCollider::Update()
	{
		auto componentModel = GetEntity()->GetComponent<ComponentModel>();

		if (componentModel != nullptr)
		{
			m_collider = componentModel->GetModel()->GetAabb();
		}
		else
		{
			m_collider = nullptr;
		}
	}

	void ComponentCollider::CmdRender(EntityRender *entityRender)
	{
	}
}
