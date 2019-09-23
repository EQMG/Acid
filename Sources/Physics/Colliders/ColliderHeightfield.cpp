#include "ColliderHeightfield.hpp"

#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include "Scenes/Entity.hpp"
#include "Physics/CollisionObject.hpp"

namespace acid {
bool ColliderHeightfield::registered = Register("colliderHeightfield");

ColliderHeightfield::ColliderHeightfield(int32_t heightStickWidth, int32_t heightStickLength, const void *heightfieldData, float minHeight,
	float maxHeight, bool flipQuadEdges, const Transform &localTransform) :
	Collider(localTransform) {
	SetHeightfield(heightStickWidth, heightStickLength, heightfieldData, minHeight, maxHeight, flipQuadEdges);
}

ColliderHeightfield::~ColliderHeightfield() {
}

void ColliderHeightfield::Start() {
}

void ColliderHeightfield::Update() {
}

btCollisionShape *ColliderHeightfield::GetCollisionShape() const {
	return m_shape.get();
}

void ColliderHeightfield::SetLocalTransform(const Transform &localTransform) {
	m_localTransform = localTransform;
	if (auto collisionObject = GetEntity()->GetComponent<CollisionObject>())
		collisionObject->SetChildTransform(this, m_localTransform);
}

void ColliderHeightfield::SetHeightfield(int32_t heightStickWidth, int32_t heightStickLength, const void *heightfieldData, float minHeight, float maxHeight,
	bool flipQuadEdges) {
	if (!heightfieldData) {
		return;
	}

	m_shape = std::make_unique<btHeightfieldTerrainShape>(heightStickWidth, heightStickLength, heightfieldData, 1.0f, minHeight, maxHeight, 1, PHY_FLOAT, flipQuadEdges);
}

const Node &operator>>(const Node &node, ColliderHeightfield &collider) {
	node["localTransform"].Get(collider.m_localTransform);
	return node;
}

Node &operator<<(Node &node, const ColliderHeightfield &collider) {
	node["localTransform"].Set(collider.m_localTransform);
	return node;
}
}
