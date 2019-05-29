#pragma once

#include "Collider.hpp"

class btHeightfieldTerrainShape;

namespace acid
{
class ColliderHeightfield :
	public Collider
{
public:
	explicit ColliderHeightfield(const int32_t &heightStickWidth = 100, const int32_t &heightStickLength = 100, const void *heightfieldData = nullptr,
		const float &minHeight = -1.0f, const float &maxHeight = 1.0f, const bool &flipQuadEdges = false, const Transform &localTransform = Transform());

	~ColliderHeightfield();

	void Start() override;

	void Update() override;

	btCollisionShape *GetCollisionShape() const override;

	void Initialize(const int32_t &heightStickWidth, const int32_t &heightStickLength, const void *heightfieldData, const float &minHeight, const float &maxHeight,
		const bool &flipQuadEdges);

	friend const Metadata &operator>>(const Metadata &metadata, ColliderHeightfield &collider);

	friend Metadata &operator<<(Metadata &metadata, const ColliderHeightfield &collider);

private:
	std::unique_ptr<btHeightfieldTerrainShape> m_shape;
};
}
