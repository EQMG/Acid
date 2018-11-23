#pragma once

#include "Collider.hpp"
#include "Maths/Vector3.hpp"
#include "Models/Model.hpp"

class btHeightfieldTerrainShape;

namespace acid
{
	class ACID_EXPORT ColliderHeightfield :
		public Collider
	{
	private:
		std::unique_ptr<btHeightfieldTerrainShape> m_shape;
	public:
		explicit ColliderHeightfield(const int32_t &heightStickWidth = 100, const int32_t &heightStickLength = 100, const void *heightfieldData = nullptr,
							const float &minHeight = -1.0f, const float &maxHeight = 1.0f, const bool &flipQuadEdges = false, const Transform &localTransform = Transform::ZERO);

		~ColliderHeightfield();

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		btCollisionShape *GetCollisionShape() const override;

		void Initialize(const int32_t &heightStickWidth, const int32_t &heightStickLength, const void *heightfieldData,
						const float &minHeight, const float &maxHeight, const bool &flipQuadEdges);
	};
}
