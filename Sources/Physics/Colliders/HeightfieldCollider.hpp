#pragma once

#include "Collider.hpp"

class btHeightfieldTerrainShape;

namespace acid {
class ACID_EXPORT HeightfieldCollider : public Collider::Registrar<HeightfieldCollider> {
	inline static const bool Registered = Register("heightfield");
public:
	explicit HeightfieldCollider(int32_t heightStickWidth = 100, int32_t heightStickLength = 100, const void *heightfieldData = nullptr,
		float minHeight = -1.0f, float maxHeight = 1.0f, bool flipQuadEdges = false, const Transform &localTransform = {});
	~HeightfieldCollider();

	btCollisionShape *GetCollisionShape() const override;

	void SetHeightfield(int32_t heightStickWidth, int32_t heightStickLength, const void *heightfieldData, float minHeight, float maxHeight,
		bool flipQuadEdges);

	friend const Node &operator>>(const Node &node, HeightfieldCollider &collider);
	friend Node &operator<<(Node &node, const HeightfieldCollider &collider);

private:
	std::unique_ptr<btHeightfieldTerrainShape> shape;
};
}
