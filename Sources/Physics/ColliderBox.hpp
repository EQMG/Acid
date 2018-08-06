#pragma once

#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include "Collider.hpp"
#include "Maths/Vector3.hpp"

namespace acid
{
	class ACID_EXPORT ColliderBox :
		public Collider
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

		btCollisionShape *GetCollisionShape() const override;

		Vector3 GetExtents() const { return m_extents; }

		void SetExtents(const Vector3 &extents) { m_extents = extents; }
	};
}
