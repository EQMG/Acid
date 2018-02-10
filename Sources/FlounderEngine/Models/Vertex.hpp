#pragma once

#include <vector>
#include "../Maths/Vector2.hpp"
#include "../Maths/Vector3.hpp"
#include "../Engine/Platform.hpp"

namespace Flounder
{
	class Vertex
	{
	public:
		Vector3 m_position;
		Vector2 m_uv;
		Vector3 m_normal;
		Vector3 m_tangent;

		Vertex(const Vector3 &position = Vector3(), const Vector2 &uv = Vector2(), const Vector3 &normal = Vector3(), const Vector3 &tangent = Vector3());

		~Vertex();

		static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions(const VkVertexInputRate &inputRate = VK_VERTEX_INPUT_RATE_VERTEX);

		static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
	};
}
