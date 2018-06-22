#include "Rigidbody.hpp"

#include "Models/Model.hpp"
#include "Scenes/Scenes.hpp"
#include "ColliderSphere.hpp"

namespace fl
{
	Rigidbody::Rigidbody(const float &mass, const float &drag, const bool &useGravity, const Constraint3 &freezePosition, const Constraint3 &freezeRotation) :
		IComponent(),
		m_mass(mass),
		m_drag(drag),
		m_useGravity(useGravity),
		m_freezePosition(freezePosition),
		m_freezeRotation(freezeRotation),
		m_colliderCopy(nullptr)
	{
	}

	Rigidbody::~Rigidbody()
	{
		delete m_colliderCopy;
	}

	void Rigidbody::Update()
	{
		auto collider = GetGameObject()->GetComponent<ICollider>();

		if (collider != nullptr)
		{
			m_colliderCopy = collider->UpdateCollider(GetGameObject()->GetTransform(), m_colliderCopy);
		}
	}

	void Rigidbody::Load(LoadedValue *value)
	{
		m_mass = value->GetChild("Mass")->Get<float>();
		m_drag = value->GetChild("Drag")->Get<float>();
		m_useGravity = value->GetChild("Use Gravity")->Get<bool>();
		m_freezePosition = value->GetChild("Freeze Position");
		m_freezeRotation = value->GetChild("Freeze Rotation");
	}

	void Rigidbody::Write(LoadedValue *destination)
	{
		destination->GetChild("Mass", true)->Set(m_mass);
		destination->GetChild("Drag", true)->Set(m_drag);
		destination->GetChild("Use Gravity", true)->Set(m_useGravity);
		m_freezePosition.Write(destination->GetChild("Freeze Position", true));
		m_freezeRotation.Write(destination->GetChild("Freeze Rotation", true));
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
			float radius = dynamic_cast<ColliderSphere *>(m_colliderCopy)->GetRadius();
			Vector3 pos = dynamic_cast<ColliderSphere *>(m_colliderCopy)->GetPosition();
			aabb1 = ColliderAabb(-radius + pos, radius + pos);
		}
		else
		{
			return result;
		}

		// Calculates the range in where there can be collisions.
		ColliderAabb collisionRange = aabb1.Stretch(amount);

		auto rigidbodyList = Scenes::Get()->GetStructure()->QueryComponents<Rigidbody>();

		Vector3 currentPosition = GetGameObject()->GetTransform().GetPosition();

		// Goes though all entities in the collision range.
		for (auto &rigidbody : rigidbodyList)
		{
			// Ignores the original entity.
			if (rigidbody->GetGameObject() == GetGameObject())
			{
				continue;
			}

			// If the main collider intersects with the other entities general collider.
			if (rigidbody->m_colliderCopy->Intersects(collisionRange).IsIntersection())
			{
				result = rigidbody->m_colliderCopy->ResolveCollision(*m_colliderCopy, currentPosition, result);
			}
		}

		// The final resulting move amount.
		return result;
	}
}
