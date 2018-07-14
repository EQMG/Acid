#pragma once

#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include "Maths/Vector3.hpp"
#include "IShape.hpp"

namespace fl
{
	class FL_EXPORT ShapeCapsule :
		public IShape
	{
	private:
		btCapsuleShape *m_shape;
	public:
		ShapeCapsule(const float &radius = 0.5f, const float &halfHeight = 0.5f);

		~ShapeCapsule();

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		std::string GetName() const override { return "ShapeCapsule"; };

		btCollisionShape *GetCollisionShape() const override { return m_shape; };
	};
}
