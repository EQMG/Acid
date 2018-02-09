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

	ComponentCollider::ComponentCollider(ComponentPrefab *prefab) :
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
			m_collider = componentModel->GetModel()->GetAabb()->Update(*m_entity->GetTransform(), m_collider);
		}
		else
		{
			delete m_collider;
		}
	}

	void ComponentCollider::CmdRender(EntityRender *entityRender)
	{
	}

	void ComponentCollider::Save(ComponentPrefab *prefab)
	{

	}
}
