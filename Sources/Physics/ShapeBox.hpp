#pragma once

#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include "Maths/Vector3.hpp"
#include "IShape.hpp"

namespace fl
{
	class FL_EXPORT ShapeBox :
		public IShape
	{
	private:
		btBoxShape *m_shape;
	public:
		ShapeBox(const Vector3 &extents = Vector3::ONE);

		~ShapeBox();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		std::string GetName() const override { return "ShapeBox"; };

		btCollisionShape *GetCollisionShape() const override { return m_shape; };

		Vector3 GetExtents() const;

		void SetExtents(const Vector3 &extents);
	};
}
