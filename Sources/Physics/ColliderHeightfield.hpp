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
		std::shared_ptr<btHeightfieldTerrainShape> m_shape;
	public:
		ColliderHeightfield(const int &heightStickWidth = 100, const int &heightStickLength = 100,
							const void *heightfieldData = nullptr, const float &heightScale = 1.0f,
							const float &minHeight = -1.0f, const float &maxHeight = 1.0f, const bool &flipQuadEdges = false);

		~ColliderHeightfield();

		void Start() override;

		void Update() override;

		void Decode(const Serialized &serialized) override;

		void Encode(Serialized &serialized) const override;

		std::shared_ptr<btCollisionShape> GetCollisionShape() const override;

		void Initialize(const int &heightStickWidth, const int &heightStickLength,
						const void *heightfieldData, const float &heightScale,
						const float &minHeight, const float &maxHeight, const bool &flipQuadEdges);
	};
}
