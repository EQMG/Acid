#pragma once

#include "Collider.hpp"

class btHeightfieldTerrainShape;

namespace acid {
class ACID_EXPORT ColliderHeightfield : public Component::Registrar<ColliderHeightfield>, public Collider {
public:
	explicit ColliderHeightfield(int32_t heightStickWidth = 100, int32_t heightStickLength = 100, const void *heightfieldData = nullptr,
		float minHeight = -1.0f, float maxHeight = 1.0f, bool flipQuadEdges = false, const Transform &localTransform = {});

	~ColliderHeightfield();

	void Start() override;
	void Update() override;

	btCollisionShape *GetCollisionShape() const override;
	void SetLocalTransform(const Transform &localTransform) override;

	void SetHeightfield(int32_t heightStickWidth, int32_t heightStickLength, const void *heightfieldData, float minHeight, float maxHeight,
		bool flipQuadEdges);

	friend const Node &operator>>(const Node &node, ColliderHeightfield &collider);
	friend Node &operator<<(Node &node, const ColliderHeightfield &collider);

private:
	static bool registered;

	std::unique_ptr<btHeightfieldTerrainShape> m_shape;
};
}
