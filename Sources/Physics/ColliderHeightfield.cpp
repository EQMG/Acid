#include "ColliderHeightfield.hpp"

#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include "Scenes/Scenes.hpp"

namespace acid
{
	ColliderHeightfield::ColliderHeightfield(const int32_t &heightStickWidth, const int32_t &heightStickLength,
											 const void *heightfieldData, const float &heightScale,
											 const float &minHeight, const float &maxHeight, const bool &flipQuadEdges) :
		Collider(),
		m_shape(nullptr)
	{
		Initialize(heightStickWidth, heightStickLength, heightfieldData,
			heightScale, minHeight, maxHeight, flipQuadEdges);
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

	void ColliderHeightfield::Decode(const Metadata &metadata)
	{
	}

	void ColliderHeightfield::Encode(Metadata &metadata) const
	{
	}

	btCollisionShape *ColliderHeightfield::GetCollisionShape() const
	{
		return m_shape.get();
	}

	void ColliderHeightfield::Initialize(const int32_t &heightStickWidth, const int32_t &heightStickLength,
										 const void *heightfieldData, const float &heightScale,
										 const float &minHeight, const float &maxHeight, const bool &flipQuadEdges)
	{
		if (heightfieldData == nullptr)
		{
			return;
		}

		m_shape = std::make_unique<btHeightfieldTerrainShape>(heightStickWidth, heightStickLength, heightfieldData,
			heightScale, minHeight, maxHeight, 1, PHY_FLOAT, flipQuadEdges);
	}
}
