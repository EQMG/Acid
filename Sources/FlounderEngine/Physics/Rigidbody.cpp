#include "Rigidbody.hpp"

#include "ColliderSphere.hpp"
#include "../Models/Model.hpp"
#include "../Scenes/Scenes.hpp"

namespace Flounder
{
	Rigidbody::Rigidbody(const float &mass, const float &drag, const bool &useGravity, const Constraint3 &freezePosition, const Constraint3 &freezeRotation) :
		Component(),
		m_mass(mass),
		m_drag(drag),
		m_useGravity(useGravity),
		m_freezePosition(new Constraint3(freezePosition)),
		m_freezeRotation(new Constraint3(freezeRotation)),
		m_colliderCopy(nullptr)
	{
		Link<float>(0, "Mass", LINK_GET(GetMass()), LINK_SET(float, SetMass(v)));
		Link<float>(1, "Drag", LINK_GET(GetDrag()), LINK_SET(float, SetDrag(v)));
		Link<bool>(2, "Use Gravity", LINK_GET(IsUseGravity()), LINK_SET(bool, SetUseGravity(v)));
		Link<bool>(3, "Freeze Position X", LINK_GET(GetFreezePosition()->m_x), LINK_SET(bool, GetFreezePosition()->m_x = v));
		Link<bool>(4, "Freeze Position Y", LINK_GET(GetFreezePosition()->m_y), LINK_SET(bool, GetFreezePosition()->m_y = v));
		Link<bool>(5, "Freeze Position Z", LINK_GET(GetFreezePosition()->m_z), LINK_SET(bool, GetFreezePosition()->m_z = v));
		Link<bool>(6, "Freeze Rotation X", LINK_GET(GetFreezeRotation()->m_x), LINK_SET(bool, GetFreezeRotation()->m_x = v));
		Link<bool>(7, "Freeze Rotation Y", LINK_GET(GetFreezeRotation()->m_y), LINK_SET(bool, GetFreezeRotation()->m_y = v));
		Link<bool>(8, "Freeze Rotation Z", LINK_GET(GetFreezeRotation()->m_z), LINK_SET(bool, GetFreezeRotation()->m_z = v));
	}

	Rigidbody::~Rigidbody()
	{
		delete m_freezePosition;
		delete m_freezeRotation;
		delete m_colliderCopy;
	}

	void Rigidbody::Update()
	{
		auto collider = GetGameObject()->GetComponent<Collider>();

		if (collider != nullptr)
		{
			m_colliderCopy = collider->Update(*GetGameObject()->GetTransform(), m_colliderCopy);
		}
	}

	Vector3 Rigidbody::ResolveCollisions(const Vector3 &amount)
	{
		// Sets the resulting resolved collisions.
		Vector3 result = Vector3(amount);

		// Verifies that this  collider will work.
		if (m_colliderCopy == nullptr)
		{
			return result;
		}

		ColliderAabb aabb1;

		if (dynamic_cast<ColliderAabb *>(m_colliderCopy) != nullptr)
		{
			aabb1 = *dynamic_cast<ColliderAabb *>(m_colliderCopy);
		}
		else if (dynamic_cast<ColliderSphere *>(m_colliderCopy) != nullptr)
		{
			const float radius = dynamic_cast<ColliderSphere *>(m_colliderCopy)->GetRadius();
			const Vector3 *pos = dynamic_cast<ColliderSphere *>(m_colliderCopy)->GetPosition();
			aabb1 = ColliderAabb(-radius + *pos, radius + *pos);
		}
		else
		{
			return result;
		}

		// Calculates the range in where there can be collisions.
		ColliderAabb collisionRange = ColliderAabb();
		ColliderAabb::Stretch(aabb1, amount, &collisionRange);

		std::vector<Rigidbody *> rigidbodys = std::vector<Rigidbody *>();
		Scenes::Get()->GetStructure()->QueryComponents<Rigidbody>(&rigidbodys);

		// Goes though all entities in the collision range.
		for (auto rigidbody : rigidbodys)
		{
			// Ignores the original entity.
			if (rigidbody->GetGameObject() == GetGameObject())
			{
				continue;
			}

			// If the main collider intersects with the other entities general collider.
			if (rigidbody->m_colliderCopy->Intersects(collisionRange).IsIntersection())
			{
				rigidbody->m_colliderCopy->ResolveCollision(*m_colliderCopy, result, &result);
			}
		}

		// The final resulting move amount.
		return result;
	}
}
