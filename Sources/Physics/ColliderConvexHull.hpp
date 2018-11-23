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
		std::unique_ptr<btConvexHullShape> m_shape;
		std::shared_ptr<Model> m_model;
		uint32_t m_points;
	public:
		explicit ColliderConvexHull(const std::vector<float> &pointCloud = {}, const Transform &localTransform = Transform::ZERO);

		~ColliderConvexHull();

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		btCollisionShape *GetCollisionShape() const override;

		uint32_t GetPoints() const { return m_points; }

		void Initialize(const std::vector<float> &pointCloud);
	};
}
