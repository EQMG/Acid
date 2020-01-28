#pragma once

#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
#include "Graphics/Pipelines/Shader.hpp"

namespace acid {
class ACID_EXPORT Vertex3 {
public:
	Vertex3() = default;
	Vertex3(const Vector3f &position, const Vector2f &uv, const Vector3f &normal) :
		m_position(position),
		m_uv(uv),
		m_normal(normal) {
	}

	static Shader::VertexInput GetVertexInput(uint32_t baseBinding = 0) {
		std::vector<VkVertexInputBindingDescription> bindingDescriptions = {
			{baseBinding, sizeof(Vertex3), VK_VERTEX_INPUT_RATE_VERTEX}
		};
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {
			{0, baseBinding, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex3, m_position)},
			{1, baseBinding, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex3, m_uv)},
			{2, baseBinding, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex3, m_normal)}
		};
		return {bindingDescriptions, attributeDescriptions};
	}

	bool operator==(const Vertex3 &other) const {
		return m_position == other.m_position && m_uv == other.m_uv && m_normal == other.m_normal;
	}

	bool operator!=(const Vertex3 &other) const {
		return !operator==(other);
	}

	Vector3f m_position;
	Vector2f m_uv;
	Vector3f m_normal;
};
}

namespace std {
template<>
struct hash<acid::Vertex3> {
	size_t operator()(const acid::Vertex3 &vertex) const noexcept {
		size_t seed = 0;
		acid::Maths::HashCombine(seed, vertex.m_position);
		acid::Maths::HashCombine(seed, vertex.m_uv);
		acid::Maths::HashCombine(seed, vertex.m_normal);
		return seed;
	}
};
}
