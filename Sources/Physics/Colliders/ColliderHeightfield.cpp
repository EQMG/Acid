#include "ColliderHeightfield.hpp"

#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

namespace acid {
ColliderHeightfield::ColliderHeightfield(int32_t heightStickWidth, int32_t heightStickLength, const void *heightfieldData, float minHeight,
	float maxHeight, bool flipQuadEdges, const Transform &localTransform) /*:
	Collider(localTransform)*/ {
	this->localTransform = localTransform;
	SetHeightfield(heightStickWidth, heightStickLength, heightfieldData, minHeight, maxHeight, flipQuadEdges);
}

ColliderHeightfield::~ColliderHeightfield() {
}

btCollisionShape *ColliderHeightfield::GetCollisionShape() const {
	return shape.get();
}

void ColliderHeightfield::SetHeightfield(int32_t heightStickWidth, int32_t heightStickLength, const void *heightfieldData, float minHeight, float maxHeight,
	bool flipQuadEdges) {
	if (!heightfieldData) {
		return;
	}

	shape = std::make_unique<btHeightfieldTerrainShape>(heightStickWidth, heightStickLength, heightfieldData, 1.0f, minHeight, maxHeight, 1, PHY_FLOAT, flipQuadEdges);
}

const Node &operator>>(const Node &node, ColliderHeightfield &collider) {
	node["localTransform"].Get(collider.localTransform);
	return node;
}

Node &operator<<(Node &node, const ColliderHeightfield &collider) {
	node["localTransform"].Set(collider.localTransform);
	return node;
}
}
