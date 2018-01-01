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

		static Model *Create(const float &sideLength, const float &squareSize, const int &vertexCount, const float &textureScale, const MeshType &meshType, const std::function<Vector3(float, float)> &getPosition, const std::function<Vector3(Vector3)> &getNormal, const std::function<Vector3(Vector3, Vector3)> &getColour);
	private:
		static void GenerateSimple(const int &vertexCount, std::vector<Vertex> &vertices, std::vector<uint32_t> &indices);

		static void GeneratePattern(const int &vertexCount, std::vector<Vertex> &vertices, std::vector<uint32_t> &indices);
	};
}
