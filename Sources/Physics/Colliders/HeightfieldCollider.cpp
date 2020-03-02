#include "HeightfieldCollider.hpp"

#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

namespace acid {
HeightfieldCollider::HeightfieldCollider(int32_t heightStickWidth, int32_t heightStickLength, const void *heightfieldData, float minHeight,
	float maxHeight, bool flipQuadEdges, const Transform &localTransform) /*:
	Collider(localTransform)*/ {
	this->localTransform = localTransform;
	SetHeightfield(heightStickWidth, heightStickLength, heightfieldData, minHeight, maxHeight, flipQuadEdges);
}

HeightfieldCollider::~HeightfieldCollider() {
}

btCollisionShape *HeightfieldCollider::GetCollisionShape() const {
	return shape.get();
}

void HeightfieldCollider::SetHeightfield(int32_t heightStickWidth, int32_t heightStickLength, const void *heightfieldData, float minHeight, float maxHeight,
	bool flipQuadEdges) {
	if (!heightfieldData) {
		return;
	}

	shape = std::make_unique<btHeightfieldTerrainShape>(heightStickWidth, heightStickLength, heightfieldData, 1.0f, minHeight, maxHeight, 1, PHY_FLOAT, flipQuadEdges);
}

const Node &operator>>(const Node &node, HeightfieldCollider &collider) {
	node["localTransform"].Get(collider.localTransform);
	return node;
}

Node &operator<<(Node &node, const HeightfieldCollider &collider) {
	node["localTransform"].Set(collider.localTransform);
	return node;
}
}
