#pragma once

#include <BulletCollision/CollisionShapes/btConeShape.h>
#include "Maths/Vector3.hpp"
#include "IShape.hpp"

namespace fl
{
	class FL_EXPORT ShapeCone :
		public IShape
	{
	private:
		btConeShape *m_shape;
	public:
		ShapeCone(const float &radius = 0.5f, const float &height = 0.5f);

		~ShapeCone();

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		std::string GetName() const override { return "ShapeCone"; };

		btCollisionShape *GetCollisionShape() const override { return m_shape; };
	};
}
