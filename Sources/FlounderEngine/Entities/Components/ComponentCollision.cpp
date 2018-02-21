#include "ComponentCollision.hpp"

#include "../../Physics/Sphere.hpp"
#include "../Entity.hpp"
#include "../Entities.hpp"
#include "ComponentCollider.hpp"
#include "ComponentModel.hpp"

namespace Flounder
{
	ComponentCollision::ComponentCollision() :
		Component()
	{
	}

	ComponentCollision::ComponentCollision(ComponentPrefab *prefab) :
		Component()
	{
	}

	ComponentCollision::~ComponentCollision()
	{
	}

	void ComponentCollision::Update()
	{
		auto componentModel = GetGameObject()->GetComponent<ComponentModel>();

		if (componentModel != nullptr)
		{
			// TODO: Load quickhull.
		}
	}

	void ComponentCollision::CmdRender(EntityRender *entityRender)
	{
	}

//	void ComponentCollision::Save(ComponentPrefab *prefab)
//	{
//
//	}

	Vector3 ComponentCollision::ResolveCollisions(const Vector3 &amount)
	{
		// Sets the resulting resolved collisions.
		Vector3 result = Vector3(amount);

		// Gets this entities collider.
		auto componentCollider1 = GetGameObject()->GetComponent<ComponentCollider>();

		// Verifies that this entities main collider will work.
		if (componentCollider1 == nullptr)
		{
			return result;
		}

		Aabb aabb1;

		if (dynamic_cast<Aabb *>(componentCollider1->GetCollider()) != nullptr)
		{
			aabb1 = *dynamic_cast<Aabb *>(componentCollider1->GetCollider());
		}
		else if (dynamic_cast<Sphere *>(componentCollider1->GetCollider()) != nullptr)
		{
			const float radius = dynamic_cast<Sphere *>(componentCollider1->GetCollider())->GetRadius();
			const Vector3 *pos = dynamic_cast<Sphere *>(componentCollider1->GetCollider())->GetPosition();
			aabb1 = Aabb(-radius + *pos, radius + *pos);
		}
		else
		{
			return result;
		}

		// Calculates the range in where there can be collisions.
		Aabb collisionRange = Aabb();
		Aabb::Stretch(aabb1, amount, &collisionRange);

		// Goes though all entities in the collision range.
		for (auto entity : *Entities::Get()->GetStructure()->GetAll())
		{
			// Ignores the original entity.
			if (entity == GetGameObject())
			{
				continue;
			}

			// Gets the checked entities collider.
			auto componentCollider2 = entity->GetComponent<ComponentCollider>();

			// Verifies that the checked entities main collider will work.
			if (componentCollider2 == nullptr)
			{
				continue;
			}

			// If the main collider intersects with the other entities general collider.
			if (componentCollider2->GetCollider()->Intersects(collisionRange).IsIntersection())
			{
				// If the main colliders are the only ones use them.
				// bool hull1 = componentCollider1->GetQuickHull() != nullptr && componentCollider1->GetQuickHull()->IsLoaded();
				// bool hull2 = componentCollider2->GetQuickHull() != nullptr && componentCollider2->GetQuickHull()->IsLoaded();
				//Collider *colliderLeft = hull1 ? componentCollider1->GetQuickHull() : collider1;
				//Collider *colliderRight = hull2 ? componentCollider2->GetQuickHull() : collider2;
				//colliderLeft->ResolveCollision(colliderRight, result, &result);

				//if (hullLeft && hullRight) { // TODO: Make Quick Hull collisions work!
				//	componentCollider1.getQuickHull().resolveCollision(componentCollider2.getQuickHull(), result, result);
				//} else {
				componentCollider1->GetCollider()->ResolveCollision(*componentCollider2->GetCollider(), result, &result);
				//}
			}
		}

		// The final resulting move amount.
		return result;
	}
}
