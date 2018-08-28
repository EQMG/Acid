#include "ColliderHeightfield.hpp"

#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include "Scenes/Scenes.hpp"

namespace acid
{
	ColliderHeightfield::ColliderHeightfield(const int &heightStickWidth, const int &heightStickLength,
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
		delete m_shape;
	}

	void ColliderHeightfield::Start()
	{
	}

	void ColliderHeightfield::Update()
	{
	}

	void ColliderHeightfield::Load(LoadedValue *value)
	{
	}

	void ColliderHeightfield::Write(LoadedValue *destination)
	{
	}

	btCollisionShape *ColliderHeightfield::GetCollisionShape() const
	{
		return m_shape;
	}

	void ColliderHeightfield::Initialize(const int &heightStickWidth, const int &heightStickLength,
										 const void *heightfieldData, const float &heightScale,
										 const float &minHeight, const float &maxHeight, const bool &flipQuadEdges)
	{
		delete m_shape;

		if (heightfieldData == nullptr)
		{
			return;
		}

		m_shape = new btHeightfieldTerrainShape(heightStickWidth, heightStickLength, heightfieldData,
			heightScale, minHeight, maxHeight, 1, PHY_FLOAT, flipQuadEdges);
	}
}
