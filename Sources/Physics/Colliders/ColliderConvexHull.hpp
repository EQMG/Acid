#pragma once

#include "Collider.hpp"
#include "Models/Model.hpp"

class btConvexHullShape;

namespace acid
{
class ACID_EXPORT ColliderConvexHull : public Collider
{
  public:
	explicit ColliderConvexHull(const std::vector<float>& pointCloud = {}, const Transform& localTransform = Transform::Identity);

	~ColliderConvexHull();

	void Start() override;

	void Update() override;

	void Decode(const Metadata& metadata) override;

	void Encode(Metadata& metadata) const override;

	btCollisionShape* GetCollisionShape() const override;

	const uint32_t& GetPointCount() const
	{
		return m_pointCount;
	}

	void Initialize(const std::vector<float>& pointCloud);

  private:
	std::unique_ptr<btConvexHullShape> m_shape;
	std::shared_ptr<Model> m_model;
	uint32_t m_pointCount;
};
}
