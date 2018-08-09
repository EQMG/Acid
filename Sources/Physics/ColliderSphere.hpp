#pragma once

#include "Collider.hpp"

class btSphereShape;

namespace acid
{
	class ACID_EXPORT ColliderSphere :
		public Collider
	{
	private:
		btSphereShape *m_shape;
		float m_radius;
	public:
		ColliderSphere(const float &radius = 1.0f);

		~ColliderSphere();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		btCollisionShape *GetCollisionShape() const override;

		float GetRadius() const { return m_radius; }

		void SetRadius(const float &radius) { m_radius = radius; }
	};
}
