#include "ColliderHeightfield.hpp"

#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include "Scenes/Scenes.hpp"

namespace acid
{
	ColliderHeightfield::ColliderHeightfield(const int &heightStickWidth, const int &heightStickLength,
											 const void *heightfieldData, const float &heightScale,
											 const float &minHeight, const float &maxHeight,
											 const int &upAxis, const bool &flipQuadEdges) :
		m_shape(nullptr)
	{
		Initialize(heightStickWidth, heightStickLength, heightfieldData,
			heightScale, minHeight, maxHeight, upAxis, flipQuadEdges);
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
										 const float &minHeight, const float &maxHeight,
										 const int &upAxis, const bool &flipQuadEdges)
	{
		delete m_shape;

		m_shape = new btHeightfieldTerrainShape(heightStickWidth, heightStickLength, heightfieldData,
			heightScale, minHeight, maxHeight, upAxis, PHY_FLOAT, flipQuadEdges);
	}
}
