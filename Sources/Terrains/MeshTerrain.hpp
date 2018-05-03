#pragma once

#include "Models/Shapes/MeshSimple.hpp"
#include "Maths/Maths.hpp"
#include "Maths/Colour.hpp"

namespace fl
{
	class FL_EXPORT MeshTerrain :
		public MeshSimple
	{
	private:
		float m_radius;
		Transform *m_transform;
	public:
		static const int SIDE_LENGTH;
		static const std::vector<float> SQUARE_SIZES;
		static const std::vector<float> TEXTURE_SCALES;

		MeshTerrain(const float &sideLength, const float &squareSize, const int &vertexCount, const float &textureScale, const float &radius, Transform *transform);

		~MeshTerrain();

		Vector3 GetPosition(const float &x, const float &z) override;

		Vector3 GetNormal(const Vector3 &position) override;

		Vector3 GetColour(const Vector3 &position, const Vector3 &normal) override;
	};
}