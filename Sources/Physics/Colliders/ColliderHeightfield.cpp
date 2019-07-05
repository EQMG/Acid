#include "ColliderHeightfield.hpp"

#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include "Scenes/Entity.hpp"

namespace acid
{
ColliderHeightfield::ColliderHeightfield(const int32_t &heightStickWidth, const int32_t &heightStickLength, const void *heightfieldData, const float &minHeight,
	const float &maxHeight, const bool &flipQuadEdges, const Transform &localTransform) :
	Collider{localTransform}
{
	Initialize(heightStickWidth, heightStickLength, heightfieldData, minHeight, maxHeight, flipQuadEdges);
}

ColliderHeightfield::~ColliderHeightfield()
{
}

void ColliderHeightfield::Start()
{
}

void ColliderHeightfield::Update()
{
}

btCollisionShape *ColliderHeightfield::GetCollisionShape() const
{
	return m_shape.get();
}

void ColliderHeightfield::Initialize(const int32_t &heightStickWidth, const int32_t &heightStickLength, const void *heightfieldData, const float &minHeight, const float &maxHeight,
	const bool &flipQuadEdges)
{
	if (heightfieldData == nullptr)
	{
		return;
	}

	m_shape = std::make_unique<btHeightfieldTerrainShape>(heightStickWidth, heightStickLength, heightfieldData, 1.0f, minHeight, maxHeight, 1, PHY_FLOAT, flipQuadEdges);
}

const Node &operator>>(const Node &node, ColliderHeightfield &collider)
{
	node["localTransform"].Get(collider.m_localTransform);
	return node;
}

Node &operator<<(Node &node, const ColliderHeightfield &collider)
{
	node["localTransform"].Set(collider.m_localTransform);
	return node;
}
}
