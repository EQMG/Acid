#pragma once

#include "Models/Model.hpp"
#include "Collider.hpp"

class btConvexHullShape;

namespace acid
{
class ACID_EXPORT ColliderConvexHull :
	public Collider
{
public:
	explicit ColliderConvexHull(const std::vector<float> &pointCloud = {}, const Transform &localTransform = Transform());

	~ColliderConvexHull();

	void Start() override;

	void Update() override;

	btCollisionShape *GetCollisionShape() const override;

	const uint32_t &GetPointCount() const { return m_pointCount; }

	void Initialize(const std::vector<float> &pointCloud);

	friend const Metadata &operator>>(const Metadata &metadata, ColliderConvexHull &collider);

	friend Metadata &operator<<(Metadata &metadata, const ColliderConvexHull &collider);

private:
	std::unique_ptr<btConvexHullShape> m_shape;
	std::shared_ptr<Model> m_model;
	uint32_t m_pointCount{};
};
}
