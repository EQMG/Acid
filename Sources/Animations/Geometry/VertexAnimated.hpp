#pragma once

#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
#include "Renderer/Pipelines/Pipeline.hpp"

namespace acid
{
class ACID_EXPORT VertexAnimated
{
public:
	VertexAnimated(const Vector3f &position, const Vector2f &uv, const Vector3f &normal, const Vector3ui &jointId, const Vector3f &vertexWeight) :
		m_position(position),
		m_uv(uv),
		m_normal(normal),
		m_jointId(jointId),
		m_vertexWeight(vertexWeight)
	{
	}

	bool operator==(const VertexAnimated &other) const
	{
		return m_position == other.m_position && m_uv == other.m_uv && m_normal == other.m_normal && m_jointId == other.m_jointId && m_vertexWeight == other.m_vertexWeight;
	}

	bool operator!=(const VertexAnimated &other) const
	{
		return !(*this == other);
	}

	static Shader::VertexInput GetVertexInput(const uint32_t &baseBinding = 0)
	{
		std::vector<VkVertexInputBindingDescription> bindingDescriptions = { 
			VkVertexInputBindingDescription{ baseBinding, sizeof(VertexAnimated), VK_VERTEX_INPUT_RATE_VERTEX }
		};
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {
			VkVertexInputAttributeDescription{ 0, baseBinding, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexAnimated, m_position) },
			VkVertexInputAttributeDescription{ 1, baseBinding, VK_FORMAT_R32G32_SFLOAT, offsetof(VertexAnimated, m_uv) },
			VkVertexInputAttributeDescription{ 2, baseBinding, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexAnimated, m_normal) },
			VkVertexInputAttributeDescription{ 3, baseBinding, VK_FORMAT_R32G32B32_SINT, offsetof(VertexAnimated, m_jointId) },
			VkVertexInputAttributeDescription{ 4, baseBinding, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexAnimated, m_vertexWeight) }
		};
		return Shader::VertexInput(bindingDescriptions, attributeDescriptions);
	}

	Vector3f m_position;
	Vector2f m_uv;
	Vector3f m_normal;
	Vector3ui m_jointId;
	Vector3f m_vertexWeight;
};
}

namespace std
{
template<>
struct hash<acid::VertexAnimated>
{
	size_t operator()(acid::VertexAnimated const &vertex) const noexcept
	{
		size_t seed = 0;
		acid::Maths::HashCombine(seed, vertex.m_position);
		acid::Maths::HashCombine(seed, vertex.m_uv);
		acid::Maths::HashCombine(seed, vertex.m_normal);
		acid::Maths::HashCombine(seed, vertex.m_jointId);
		acid::Maths::HashCombine(seed, vertex.m_vertexWeight);
		return seed;
	}
};
}
