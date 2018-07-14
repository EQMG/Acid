#pragma once

#include <BulletCollision/CollisionShapes/btCylinderShape.h>
#include "Maths/Vector3.hpp"
#include "IShape.hpp"

namespace fl
{
	class FL_EXPORT ShapeCylinder :
		public IShape
	{
	private:
		btCylinderShape *m_shape;
	public:
		ShapeCylinder(const Vector3 &halfExtents = Vector3::ONE);

		~ShapeCylinder();

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		std::string GetName() const override { return "ShapeCylinder"; };

		btCollisionShape *GetCollisionShape() const override { return m_shape; };
	};
}
