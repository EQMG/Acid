#pragma once

#include "Graphics/Pipelines/Shader.hpp"

namespace acid {
class ACID_EXPORT Vertex3d {
public:
	Vertex3d() = default;
	Vertex3d(const Vector3f &position, const Vector2f &uv, const Vector3f &normal) :
		position(position),
		uv(uv),
		normal(normal) {
	}

	static Shader::VertexInput GetVertexInput(uint32_t baseBinding = 0) {
		std::vector<VkVertexInputBindingDescription> bindingDescriptions = {
			{baseBinding, sizeof(Vertex3d), VK_VERTEX_INPUT_RATE_VERTEX}
		};
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {
			{0, baseBinding, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex3d, position)},
			{1, baseBinding, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex3d, uv)},
			{2, baseBinding, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex3d, normal)}
		};
		return {bindingDescriptions, attributeDescriptions};
	}

	bool operator==(const Vertex3d &rhs) const {
		return position == rhs.position && uv == rhs.uv && normal == rhs.normal;
	}

	bool operator!=(const Vertex3d &rhs) const {
		return !operator==(rhs);
	}

	Vector3f position;
	Vector2f uv;
	Vector3f normal;
};
}

namespace std {
template<>
struct hash<acid::Vertex3d> {
	size_t operator()(const acid::Vertex3d &vertex) const noexcept {
		size_t seed = 0;
		acid::Maths::HashCombine(seed, vertex.position);
		acid::Maths::HashCombine(seed, vertex.uv);
		acid::Maths::HashCombine(seed, vertex.normal);
		return seed;
	}
};
}
