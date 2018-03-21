#pragma once

#include <vector>
#include "../../../Maths/Vector2.hpp"
#include "../../../Maths/Vector3.hpp"
#include "../../../Engine/Platform.hpp"
#include "../../../Models/IVertex.hpp"

namespace Flounder
{
	class F_EXPORT VertexAnimated :
		public IVertex
	{
	public:
		Vector3 m_position;
		Vector2 m_uv;
		Vector3 m_normal;
		Vector3 m_tangent;
		Vector3 m_jointId;
		Vector3 m_vertexWeight;

		VertexAnimated(const Vector3 &position = Vector3(), const Vector2 &uv = Vector2(), const Vector3 &normal = Vector3(), const Vector3 &tangent = Vector3(), const Vector3 &jointId = Vector3(), const Vector3 &vertexWeight = Vector3());

		VertexAnimated(const VertexAnimated &source);

		~VertexAnimated();

		static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions(const VkVertexInputRate &inputRate = VK_VERTEX_INPUT_RATE_VERTEX);

		static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions(const int &usedCount = 5);
	};
}
