#pragma once

#include "Maths/Vector3.hpp"
#include "Models/Model.hpp"
#include "Collider.hpp"

class btHeightfieldTerrainShape;

namespace acid
{
	class ACID_EXPORT ColliderHeightfield :
		public Collider
	{
	private:
		btHeightfieldTerrainShape *m_shape;
	public:
		ColliderHeightfield(const int &heightStickWidth, const int &heightStickLength,
							const void *heightfieldData, const float &heightScale,
							const float &minHeight, const float &maxHeight, const bool &flipQuadEdges);

		~ColliderHeightfield();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		std::string GetName() const override { return "ColliderHeightfield"; };

		ACID_HIDDEN btCollisionShape *GetCollisionShape() const override;

		void Initialize(const int &heightStickWidth, const int &heightStickLength,
						const void *heightfieldData, const float &heightScale,
						const float &minHeight, const float &maxHeight, const bool &flipQuadEdges);
	};
}
