#pragma once

#include <Models/Shapes/MeshSimple.hpp>
#include <Maths/Maths.hpp>
#include <Maths/Colour.hpp>

using namespace fl;

namespace test
{
	class MeshTerrain :
		public MeshSimple
	{
	private:
		Transform *m_transform;
	public:
		static const int SIDE_LENGTH;
		static const std::vector<float> SQUARE_SIZES;
		static const std::vector<float> TEXTURE_SCALES;

		MeshTerrain(const float &sideLength, const float &squareSize, const int &vertexCount, const float &textureScale, Transform *transform);

		~MeshTerrain();

		Vector3 GetPosition(const float &x, const float &z) override;

		Vector3 GetNormal(const Vector3 &position) override;

		Vector3 GetColour(const Vector3 &position, const Vector3 &normal) override;
	private:
		static float GetHeight(const float &x, const float &z);

		static Vector3 GetNormal(const float &x, const float &z);
	};
}