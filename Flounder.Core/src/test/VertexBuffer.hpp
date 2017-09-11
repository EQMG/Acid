#pragma once

#include <iostream>
#include <vector>

#include "Vertex.hpp"

namespace Flounder
{
	class VertexBuffer
	{
	private:
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;

		const std::vector<Vertex> vertices =
		{
			{ Vector2(0.0f, -0.5f), Colour(1.0f, 0.0f, 0.0f, 1.0f) },
			{ Vector2(0.5f, 0.5f), Colour(0.0f, 1.0f, 0.0, 1.0f) },
			{ Vector2(-0.5f, 0.5f), Colour(0.0f, 0.0f, 1.0f, 1.0f) }
		};
	public:
		VertexBuffer();

		~VertexBuffer();

		void Create();
		
		void Cleanup();

		VkBuffer GetVertexBuffer() const { return vertexBuffer; }

		uint32_t GetVerticesSize() const { return vertices.size(); }
	private:
		uint32_t FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
	};
}
