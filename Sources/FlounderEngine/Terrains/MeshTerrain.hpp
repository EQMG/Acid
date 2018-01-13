#pragma once

#include "Models/Shapes/MeshSimple.hpp"

namespace Flounder
{
	class MeshTerrain :
		public MeshSimple
	{
	private:
		Vector3 *m_position;
	public:
		MeshTerrain(const float &sideLength, const float &squareSize, const int &vertexCount, const float &textureScale, Vector3 *position);

		Vector3 GetPosition(const float &x, const float &z) override;

		Vector3 GetNormal(const Vector3 &position) override;

		Vector3 GetColour(const Vector3 &position, const Vector3 &normal) override;
	};
}