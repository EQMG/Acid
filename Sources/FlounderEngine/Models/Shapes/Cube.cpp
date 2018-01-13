#include "Cube.hpp"

#include <algorithm>
#include "Maths/Maths.hpp"

namespace Flounder
{
	Cube::Cube(const float &width, const float &height, const float &depth) :
		Model()
	{
		std::vector<Vertex> vertices = {
			Vertex(Vector3(-0.5f, 0.5f, -0.5f), Vector2(0.0f, 0.66f), Vector3(0.0f, 0.0f, -1.0f)),
			Vertex(Vector3(-0.5f, -0.5f, -0.5f), Vector2(0.25f, 0.66f), Vector3(0.0f, 0.0f, -1.0f)),
			Vertex(Vector3(0.5f, 0.5f, -0.5f), Vector2(0.0f, 0.33f), Vector3(0.0f, 0.0f, -1.0f)),
			Vertex(Vector3(0.5f, -0.5f, -0.5f), Vector2(0.25f, 0.33f), Vector3(0.0f, 0.0f, -1.0f)),

			Vertex(Vector3(-0.5f, -0.5f, 0.5f), Vector2(0.5f, 0.66f), Vector3(0.0f, 0.0f, 1.0f)),
			Vertex(Vector3(0.5f, -0.5f, 0.5f), Vector2(0.5f, 0.33f), Vector3(0.0f, 0.0f, 1.0f)),
			Vertex(Vector3(-0.5f, 0.5f, 0.5f), Vector2(0.75f, 0.66f), Vector3(0.0f, 0.0f, 1.0f)),
			Vertex(Vector3(0.5f, 0.5f, 0.5f), Vector2(0.75f, 0.33f), Vector3(0.0f, 0.0f, 1.0f)),

			Vertex(Vector3(-0.5f, 0.5f, -0.5f), Vector2(1.0f, 0.66f), Vector3(0.0f, 1.0f, 0.0f)),
			Vertex(Vector3(0.5f, 0.5f, -0.5f), Vector2(1.0f, 0.33f), Vector3(0.0f, 1.0f, 0.0f)),

			Vertex(Vector3(-0.5f, 0.5f, -0.5f), Vector2(0.25f, 1.0f), Vector3(0.0f, -1.0f, 0.0f)),
			Vertex(Vector3(-0.5f, 0.5f, 0.5f), Vector2(0.5f, 1.0f), Vector3(0.0f, -1.0f, 0.0f)),

			Vertex(Vector3(0.5f, 0.5f, -0.5f), Vector2(0.25f, 0.0f), Vector3(1.0f, 0.0f, 0.0f)),
			Vertex(Vector3(0.5f, 0.5f, 0.5f), Vector2(0.5f, 0.0f), Vector3(1.0f, 0.0f, 0.0f)),
		};
		std::vector<uint32_t> indices = {
			0, 2, 1, // Front
			1, 2, 3,
			4, 5, 6, // Back
			5, 7, 6,
			6, 7, 8, // Top
			7, 9 ,8,
			1, 3, 4, // Bottom
			3, 5, 4,
			1, 11, 10, // Left
			1, 4, 11,
			3, 12, 5, // Right
			5, 12, 13
		};

		for (Vertex vertex : vertices)
		{
			vertex.m_position.m_x *= width;
			vertex.m_position.m_y *= height;
			vertex.m_position.m_z *= depth;
		}

		Model::Set(vertices, indices);
	}
}