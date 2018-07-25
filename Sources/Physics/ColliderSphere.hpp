#pragma once

#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include "ICollider.hpp"

namespace acid
{
	class FL_EXPORT ColliderSphere :
		public ICollider
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

		std::string GetName() const override { return "ColliderSphere"; };

		btCollisionShape *GetCollisionShape() const override { return m_shape; };

		float GetRadius() const { return m_radius; }

		void SetRadius(const float &radius) { m_radius = radius; }
	};
}
