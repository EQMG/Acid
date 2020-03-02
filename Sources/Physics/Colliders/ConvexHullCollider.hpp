#pragma once

#include "Models/Model.hpp"
#include "Collider.hpp"

class btConvexHullShape;

namespace acid {
class ACID_EXPORT ConvexHullCollider : public Collider::Registrar<ConvexHullCollider> {
	inline static const bool Registered = Register("convexHull");
public:
	explicit ConvexHullCollider(const std::vector<float> &pointCloud = {}, const Transform &localTransform = {});
	~ConvexHullCollider();

	btCollisionShape *GetCollisionShape() const override;

	uint32_t GetPointCount() const { return pointCount; }
	void SetPointCount(const std::vector<float> &pointCloud);

	friend const Node &operator>>(const Node &node, ConvexHullCollider &collider);
	friend Node &operator<<(Node &node, const ConvexHullCollider &collider);

private:
	std::unique_ptr<btConvexHullShape> shape;
	std::shared_ptr<Model> model;
	uint32_t pointCount = 0;
};
}
