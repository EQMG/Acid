#pragma once

#include <BulletCollision/CollisionShapes/btConvexHullShape.h>
#include "Maths/Vector3.hpp"
#include "Models/Model.hpp"
#include "ICollider.hpp"

namespace acid
{
	class FL_EXPORT ColliderConvexHull :
		public ICollider
	{
	private:
		btConvexHullShape *m_shape;
		std::shared_ptr<Model> m_model;
		int m_points;
	public:
		ColliderConvexHull(const std::vector<float> &pointCloud = std::vector<float>());

		~ColliderConvexHull();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		std::string GetName() const override { return "ColliderConvexHull"; };

		btCollisionShape *GetCollisionShape() const override { return m_shape; };

		int GetPoints() const { return m_points; }

		void SetPointCloud(const std::vector<float> &pointCloud);
	};
}
