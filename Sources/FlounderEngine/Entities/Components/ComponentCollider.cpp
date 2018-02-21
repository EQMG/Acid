#include "ComponentCollider.hpp"

#include "../Entity.hpp"
#include "ComponentModel.hpp"

namespace Flounder
{
	ComponentCollider::ComponentCollider() :
		Component(),
		m_collider(nullptr)
	{
	}

	ComponentCollider::ComponentCollider(ComponentPrefab *prefab) :
		Component(),
		m_collider(nullptr)
	{
	}

	ComponentCollider::~ComponentCollider()
	{
	}

	void ComponentCollider::Update()
	{
		auto componentModel = GetGameObject()->GetComponent<ComponentModel>();

		if (componentModel != nullptr)
		{
			m_collider = componentModel->GetModel()->GetAabb()->Update(*GetGameObject()->GetTransform(), m_collider);
		}
		else
		{
			delete m_collider;
		}
	}

	void ComponentCollider::CmdRender(EntityRender *entityRender)
	{
	}

//	void ComponentCollider::Save(ComponentPrefab *prefab)
//	{
//
//	}
}
