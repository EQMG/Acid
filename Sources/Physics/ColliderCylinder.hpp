#pragma once

#include <BulletCollision/CollisionShapes/btCylinderShape.h>
#include "Maths/Vector3.hpp"
#include "ICollider.hpp"

namespace fl
{
	class FL_EXPORT ColliderCylinder :
		public ICollider
	{
	private:
		btCylinderShape *m_shape;
		float m_radius;
		float m_height;
	public:
		ColliderCylinder(const float &radius = 1.0f, const float &height = 1.0f);

		~ColliderCylinder();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		std::string GetName() const override { return "ColliderCylinder"; };

		btCollisionShape *GetCollisionShape() const override { return m_shape; };

		float GetRadius() const { return m_radius; }

		void SetRadius(const float &radius) { m_radius = radius; }

		float GetHeight() const { return m_height;}

		void SetHeight(const float &height) { m_height = height; }
	};
}
