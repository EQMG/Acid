#pragma once

#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
#include "Graphics/Pipelines/Shader.hpp"

namespace acid {
class ACID_EXPORT Vertex3d {
public:
	Vertex3d() = default;
	Vertex3d(const Vector3f &position, const Vector2f &uv, const Vector3f &normal) :
		m_position(position),
		m_uv(uv),
		m_normal(normal) {
	}

	static Shader::VertexInput GetVertexInput(uint32_t baseBinding = 0) {
		std::vector<VkVertexInputBindingDescription> bindingDescriptions = {
			{baseBinding, sizeof(Vertex3d), VK_VERTEX_INPUT_RATE_VERTEX}
		};
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {
			{0, baseBinding, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex3d, m_position)},
			{1, baseBinding, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex3d, m_uv)},
			{2, baseBinding, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex3d, m_normal)}
		};
		return {bindingDescriptions, attributeDescriptions};
	}

	bool operator==(const Vertex3d &other) const {
		return m_position == other.m_position && m_uv == other.m_uv && m_normal == other.m_normal;
	}

	bool operator!=(const Vertex3d &other) const {
		return !operator==(other);
	}

	Vector3f m_position;
	Vector2f m_uv;
	Vector3f m_normal;
};
}

namespace std {
template<>
struct hash<acid::Vertex3d> {
	size_t operator()(const acid::Vertex3d &vertex) const noexcept {
		size_t seed = 0;
		acid::Maths::HashCombine(seed, vertex.m_position);
		acid::Maths::HashCombine(seed, vertex.m_uv);
		acid::Maths::HashCombine(seed, vertex.m_normal);
		return seed;
	}
};
}
