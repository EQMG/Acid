#include "Rigidbody.hpp"

#include "Models/Model.hpp"
#include "Scenes/Scenes.hpp"
#include "ColliderSphere.hpp"

namespace fl
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
	}

	Rigidbody::~Rigidbody()
	{
		delete m_freezePosition;
		delete m_freezeRotation;
		delete m_colliderCopy;
	}

	void Rigidbody::Update()
	{
		auto collider = GetGameObject()->GetComponent<ICollider>();

		if (collider != nullptr)
		{
			m_colliderCopy = collider->UpdateCollider(*GetGameObject()->GetTransform(), m_colliderCopy);
		}
	}

	void Rigidbody::Load(LoadedValue *value)
	{
		m_mass = value->GetChild("Mass")->Get<float>();
		m_drag = value->GetChild("Drag")->Get<float>();
		m_useGravity = value->GetChild("Use Gravity")->Get<bool>();
		*m_freezePosition = value->GetChild("Freeze Position");
		*m_freezeRotation = value->GetChild("Freeze Rotation");
	}

	void Rigidbody::Write(LoadedValue *value)
	{
		value->GetChild("Mass", true)->Set(m_mass);
		value->GetChild("Drag", true)->Set(m_drag);
		value->GetChild("Use Gravity", true)->Set(m_useGravity);
		m_freezePosition->Write(value->GetChild("Freeze Position", true));
		m_freezeRotation->Write(value->GetChild("Freeze Rotation", true));
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
