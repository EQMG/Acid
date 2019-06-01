#pragma once

#include "Maths/Colour.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
#include "Graphics/Pipelines/Pipeline.hpp"

namespace acid
{
class ACID_EXPORT VertexDefault
{
public:
	VertexDefault(const Vector3f &position, const Vector2f &uv, const Vector3f &normal) :
		m_position(position),
		m_uv(uv),
		m_normal(normal)
	{
	}

	bool operator==(const VertexDefault &other) const
	{
		return m_position == other.m_position && m_uv == other.m_uv && m_normal == other.m_normal;
	}

	bool operator!=(const VertexDefault &other) const
	{
		return !(*this == other);
	}

	static Shader::VertexInput GetVertexInput(const uint32_t &baseBinding = 0)
	{
		std::vector<VkVertexInputBindingDescription> bindingDescriptions = { 
			VkVertexInputBindingDescription{ baseBinding, sizeof(VertexDefault), VK_VERTEX_INPUT_RATE_VERTEX }
		};
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {
			VkVertexInputAttributeDescription{ 0, baseBinding, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexDefault, m_position) },
			VkVertexInputAttributeDescription{ 1, baseBinding, VK_FORMAT_R32G32_SFLOAT, offsetof(VertexDefault, m_uv) },
			VkVertexInputAttributeDescription{ 2, baseBinding, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexDefault, m_normal) }
		};
		return Shader::VertexInput(bindingDescriptions, attributeDescriptions);
	}

	Vector3f m_position;
	Vector2f m_uv;
	Vector3f m_normal;
};
}

namespace std
{
template<>
struct hash<acid::VertexDefault>
{
	size_t operator()(const acid::VertexDefault &vertex) const noexcept
	{
		size_t seed = 0;
		acid::Maths::HashCombine(seed, vertex.m_position);
		acid::Maths::HashCombine(seed, vertex.m_uv);
		acid::Maths::HashCombine(seed, vertex.m_normal);
		return seed;
	}
};
}
