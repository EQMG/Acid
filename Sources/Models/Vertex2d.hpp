#pragma once

#include "Maths/Vector2.hpp"
#include "Graphics/Pipelines/Shader.hpp"

namespace acid {
class ACID_EXPORT Vertex2d {
public:
	Vertex2d() = default;
	Vertex2d(const Vector2f &position, const Vector2f &uv) :
		m_position(position),
		m_uv(uv) {
	}

	static Shader::VertexInput GetVertexInput(uint32_t baseBinding = 0) {
		std::vector<VkVertexInputBindingDescription> bindingDescriptions = {
			{baseBinding, sizeof(Vertex2d), VK_VERTEX_INPUT_RATE_VERTEX}
		};
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {
			{0, baseBinding, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex2d, m_position)},
			{1, baseBinding, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex2d, m_uv)}
		};
		return {bindingDescriptions, attributeDescriptions};
	}

	bool operator==(const Vertex2d &other) const {
		return m_position == other.m_position && m_uv == other.m_uv;
	}

	bool operator!=(const Vertex2d &other) const {
		return !operator==(other);
	}

	Vector2f m_position;
	Vector2f m_uv;
};
}

namespace std {
template<>
struct hash<acid::Vertex2d> {
	size_t operator()(const acid::Vertex2d &vertex) const noexcept {
		size_t seed = 0;
		acid::Maths::HashCombine(seed, vertex.m_position);
		acid::Maths::HashCombine(seed, vertex.m_uv);
		return seed;
	}
};
}
