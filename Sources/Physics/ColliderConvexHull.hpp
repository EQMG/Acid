#pragma once

#include "Collider.hpp"
#include "Maths/Vector3.hpp"
#include "Models/Model.hpp"

class btConvexHullShape;

namespace acid
{
	class ACID_EXPORT ColliderConvexHull :
		public Collider
	{
	private:
		btConvexHullShape *m_shape;
		std::shared_ptr<Model> m_model;
		uint32_t m_points;
	public:
		ColliderConvexHull(const std::vector<float> &pointCloud = std::vector<float>());

		~ColliderConvexHull();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		btCollisionShape *GetCollisionShape() const override;

		uint32_t GetPoints() const { return m_points; }

		void Initialize(const std::vector<float> &pointCloud);
	};
}
