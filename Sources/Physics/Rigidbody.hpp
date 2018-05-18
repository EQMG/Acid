#pragma once

#include "Maths/Constraint3.hpp"
#include "Maths/Vector3.hpp"
#include "Objects/Component.hpp"
#include "Objects/GameObject.hpp"
#include "ICollider.hpp"

namespace fl
{
	class FL_EXPORT Rigidbody :
		public Component
	{
	private:
		float m_mass;
		float m_drag;
		bool m_useGravity;
		Constraint3 *m_freezePosition;
		Constraint3 *m_freezeRotation;
		ICollider *m_colliderCopy;
	public:
		Rigidbody(const float &mass = 1.0f, const float &drag = 0.0f, const bool &useGravity = true,
				  const Constraint3 &freezePosition = Constraint3::ZERO, const Constraint3 &freezeRotation = Constraint3::ZERO);

		~Rigidbody();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *value) override;

		Vector3 ResolveCollisions(const Vector3 &amount);

		std::string GetName() const override { return "Rigidbody"; };

		float GetMass() const { return m_mass; }

		void SetMass(const float &mass) { m_mass = mass; }

		float GetDrag() const { return m_drag; }

		void SetDrag(const float &drag) { m_drag = drag; }

		bool IsUseGravity() const { return m_useGravity; }

		void SetUseGravity(const bool &useGravity) { m_useGravity = useGravity; }

		Constraint3 *GetFreezePosition() const { return m_freezePosition; }

		void SetFreezePosition(const Constraint3 &freezePosition) const { *m_freezePosition = freezePosition; }

		Constraint3 *GetFreezeRotation() const { return m_freezeRotation; }

		void SetFreezeRotation(const Constraint3 &freezeRotation) const { *m_freezeRotation = freezeRotation; }

		ICollider *GetCollider() const { return m_colliderCopy; }
	};
}
