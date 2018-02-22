#include "Rigidbody.hpp"

#include "../Physics/Sphere.hpp"
#include "../Models/Model.hpp"

namespace Flounder
{
	Rigidbody::Rigidbody(const float &mass, const float &drag, const bool &useGravity, const Constraint3 &freezePosition, const Constraint3 &freezeRotation) :
		Component(),
		m_mass(mass),
		m_drag(drag),
		m_useGravity(useGravity),
		m_freezePosition(new Constraint3(freezePosition)),
		m_freezeRotation(new Constraint3(freezeRotation))
	{
		Link<float>(0, LINK_GET(GetMass()), LINK_SET(float, SetMass(v)));
		Link<float>(1, LINK_GET(GetDrag()), LINK_SET(float, SetDrag(v)));
		Link<bool>(2, LINK_GET(IsUseGravity()), LINK_SET(bool, SetUseGravity(v)));
		Link<bool>(3, LINK_GET(GetFreezePosition()->m_x), LINK_SET(bool, GetFreezePosition()->m_x = v));
		Link<bool>(4, LINK_GET(GetFreezePosition()->m_y), LINK_SET(bool, GetFreezePosition()->m_y = v));
		Link<bool>(5, LINK_GET(GetFreezePosition()->m_z), LINK_SET(bool, GetFreezePosition()->m_z = v));
		Link<bool>(6, LINK_GET(GetFreezeRotation()->m_x), LINK_SET(bool, GetFreezeRotation()->m_x = v));
		Link<bool>(7, LINK_GET(GetFreezeRotation()->m_y), LINK_SET(bool, GetFreezeRotation()->m_y = v));
		Link<bool>(8, LINK_GET(GetFreezeRotation()->m_z), LINK_SET(bool, GetFreezeRotation()->m_z = v));
	}

	Rigidbody::~Rigidbody()
	{
		delete m_freezePosition;
		delete m_freezeRotation;
	}

	void Rigidbody::Update()
	{
		auto componentModel = GetGameObject()->GetComponent<Model>();

		if (componentModel != nullptr)
		{
			// TODO: Load quickhull.
		}
	}

	Vector3 Rigidbody::ResolveCollisions(const Vector3 &amount)
	{
		// Sets the resulting resolved collisions.
		Vector3 result = Vector3(amount);

		// Gets this entities collider.
		auto componentCollider1 = GetGameObject()->GetComponent<Collider>();

		// Verifies that this entities main collider will work.
		if (componentCollider1 == nullptr)
		{
			return result;
		}

		Aabb aabb1;

		if (dynamic_cast<Aabb *>(componentCollider1) != nullptr)
		{
			aabb1 = *dynamic_cast<Aabb *>(componentCollider1);
		}
		else if (dynamic_cast<Sphere *>(componentCollider1) != nullptr)
		{
			const float radius = dynamic_cast<Sphere *>(componentCollider1)->GetRadius();
			const Vector3 *pos = dynamic_cast<Sphere *>(componentCollider1)->GetPosition();
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
		for (auto entity : *GetGameObject()->GetStructure()->GetAll())
		{
			// Ignores the original entity.
			if (entity == GetGameObject())
			{
				continue;
			}

			// Gets the checked entities collider.
			auto componentCollider2 = entity->GetComponent<Collider>();

			// Verifies that the checked entities main collider will work.
			if (componentCollider2 == nullptr)
			{
				continue;
			}

			// If the main collider intersects with the other entities general collider.
			if (componentCollider2->Intersects(collisionRange).IsIntersection())
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
				componentCollider1->ResolveCollision(*componentCollider2, result, &result);
				//}
			}
		}

		// The final resulting move amount.
		return result;
	}
}
