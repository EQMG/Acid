#pragma once

#include <BulletCollision/CollisionShapes/btConvexHullShape.h>
#include "Maths/Vector3.hpp"
#include "IShape.hpp"

namespace fl
{
	class FL_EXPORT ShapeConvexHull :
		public IShape
	{
	private:
		btConvexHullShape *m_shape;
	public:
		ShapeConvexHull(const std::vector<Vector3> &data = std::vector<Vector3>());

		~ShapeConvexHull();

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		std::string GetName() const override { return "ShapeConvexHull"; };

		void LoadData(const std::vector<Vector3> &data);

		btCollisionShape *GetCollisionShape() const override { return m_shape; };
	};
}
