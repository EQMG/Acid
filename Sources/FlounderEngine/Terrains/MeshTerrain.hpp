#pragma once

#include "Models/Shapes/MeshSimple.hpp"

namespace Flounder
{
	class F_EXPORT MeshTerrain :
		public MeshSimple
	{
	private:
		float m_radius;
		Transform *m_transform;
		Matrix4 *m_worldMatrix;
	public:
		MeshTerrain(const float &sideLength, const float &squareSize, const int &vertexCount, const float &textureScale, const float &radius, Transform *transform);

		~MeshTerrain();

		Vector3 GetPosition(const float &x, const float &z) override;

		Vector3 GetNormal(const Vector3 &position) override;

		Vector3 GetColour(const Vector3 &position, const Vector3 &normal) override;

		Vector3 GetSphereCoords(const Vector3 &position);
	};
}