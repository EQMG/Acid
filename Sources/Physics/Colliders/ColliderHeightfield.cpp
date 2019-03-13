#include "ColliderHeightfield.hpp"

#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

namespace acid
{
ColliderHeightfield::ColliderHeightfield(const int32_t& heightStickWidth, const int32_t& heightStickLength, const void* heightfieldData, const float& minHeight, const float& maxHeight, const bool& flipQuadEdges, const Transform& localTransform)
	: Collider(localTransform), m_shape(nullptr)
{
	Initialize(heightStickWidth, heightStickLength, heightfieldData, minHeight, maxHeight, flipQuadEdges);
}

ColliderHeightfield::~ColliderHeightfield() {}

void ColliderHeightfield::Start() {}

void ColliderHeightfield::Update() {}

void ColliderHeightfield::Decode(const Metadata& metadata)
{
	metadata.GetChild("Local Transform", m_localTransform);
}

void ColliderHeightfield::Encode(Metadata& metadata) const
{
	metadata.SetChild("Local Transform", m_localTransform);
}

btCollisionShape* ColliderHeightfield::GetCollisionShape() const
{
	return m_shape.get();
}

void ColliderHeightfield::Initialize(const int32_t& heightStickWidth, const int32_t& heightStickLength, const void* heightfieldData, const float& minHeight, const float& maxHeight, const bool& flipQuadEdges)
{
	if(heightfieldData == nullptr)
		{
			return;
		}

	m_shape = std::make_unique<btHeightfieldTerrainShape>(heightStickWidth, heightStickLength, heightfieldData, 1.0f, minHeight, maxHeight, 1, PHY_FLOAT, flipQuadEdges);
}
}
