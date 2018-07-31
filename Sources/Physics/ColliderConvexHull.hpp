#pragma once

#include "Maths/Vector3.hpp"
#include "Models/Model.hpp"
#include "Collider.hpp"

class btConvexHullShape;

namespace acid
{
	class ACID_EXPORT ColliderConvexHull :
		public Collider
	{
	private:
		btConvexHullShape *m_shape;
		std::shared_ptr<Model> m_model;
		unsigned int m_points;
	public:
		ColliderConvexHull(const std::vector<float> &pointCloud = std::vector<float>());

		~ColliderConvexHull();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		std::string GetName() const override { return "ColliderConvexHull"; };

		ACID_HIDDEN btCollisionShape *GetCollisionShape() const override;

		unsigned int GetPoints() const { return m_points; }

		void Initialize(const std::vector<float> &pointCloud);
	};
}
