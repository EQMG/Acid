#pragma once

#include <Models/Shapes/MeshSimple.hpp>
#include <Maths/Maths.hpp>
#include <Maths/Matrix4.hpp>
#include <Maths/Transform.hpp>
#include <Noise/Noise.hpp>

using namespace acid;

namespace test
{
	class MeshTerrain :
		public MeshSimple
	{
	private:
		Noise *m_noise;
		Matrix4 m_worldMatrix;
	public:
		MeshTerrain(Noise *noise, const float &sideLength, const float &squareSize, const uint32_t &vertexCount, const float &textureScale, const Transform &transform);

		~MeshTerrain();

		VertexModel *GetVertex(const uint32_t &col, const uint32_t &row) override;
	private:
		Vector3 GetPosition(const float &x, const float &z);

		Vector3 GetNormal(const float &x, const float &z);
	};
}
