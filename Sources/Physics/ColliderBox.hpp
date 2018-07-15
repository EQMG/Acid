#pragma once

#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include "Maths/Vector3.hpp"
#include "ICollider.hpp"

namespace fl
{
	class FL_EXPORT ColliderBox :
		public ICollider
	{
	private:
		btBoxShape *m_shape;
		Vector3 m_extents;
	public:
		ColliderBox(const Vector3 &extents = Vector3::ONE);

		~ColliderBox();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		std::string GetName() const override { return "ColliderBox"; };

		btCollisionShape *GetCollisionShape() const override { return m_shape; };

		Vector3 GetExtents() const { return m_extents; }

		void SetExtents(const Vector3 &extents) { m_extents = extents; }
	};
}
