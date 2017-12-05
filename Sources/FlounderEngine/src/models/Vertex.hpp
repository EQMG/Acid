#pragma once

#include <vector>
#include "../maths/Vector2.hpp"
#include "../maths/Vector3.hpp"
#include "../platforms/Platform.hpp"

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

		static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();

		static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
	};
}
