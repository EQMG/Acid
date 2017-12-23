#pragma once

#include <functional>
#include "Model.hpp"

namespace Flounder
{
	class CreateMesh
	{
	public:
		enum MeshType
		{
			MeshSimple = 0,
			MeshPattern = 1
		};

		static Model *Create(const float &sideLength, const float &squareSize, const int &vertexCount, const float &textureScale, const MeshType &meshType, const std::function<float(float, float)> &getHeight, const std::function<Vector3(Vector3, Vector3)> &getColour);
	private:
		static void GenerateIndiciesSimple(const uint32_t &topLeft, const uint32_t &topRight, const uint32_t &bottomLeft, const uint32_t &bottomRight, std::vector<uint32_t> &indices);

		static void GenerateIndiciesPattern(const uint32_t &topLeft, const uint32_t &topRight, const uint32_t &bottomLeft, const uint32_t &bottomRight, const bool &sign, const bool &mixed, std::vector<uint32_t> &indices);

		static Vector3 CalculateNormal(const float &x, const float &z, const float &squareSize, const std::function<float(float, float)> &getHeight);
	};
}
