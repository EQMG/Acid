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
		ShapeConvexHull(const std::vector<float> &pointCloud = std::vector<float>());

		~ShapeConvexHull();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		std::string GetName() const override { return "ShapeConvexHull"; };

		btCollisionShape *GetCollisionShape() const override { return m_shape; };

		void SetPointCloud(const std::vector<float> &pointCloud);
	};
}
