#pragma once

#include <BulletCollision/CollisionShapes/btConeShape.h>
#include "Maths/Vector3.hpp"
#include "ICollider.hpp"

namespace acid
{
	class FL_EXPORT ColliderCone :
		public ICollider
	{
	private:
		btConeShape *m_shape;
		float m_radius;
		float m_height;
	public:
		ColliderCone(const float &radius = 1.0f, const float &height = 1.0f);

		~ColliderCone();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		std::string GetName() const override { return "ColliderCone"; };

		btCollisionShape *GetCollisionShape() const override { return m_shape; };

		float GetRadius() const { return m_radius; }

		void SetRadius(const float &radius) { m_radius = radius; }

		float GetHeight() const { return m_height;}

		void SetHeight(const float &height) { m_height = height; }
	};
}
