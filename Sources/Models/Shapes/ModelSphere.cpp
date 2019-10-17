#include "ModelSphere.hpp"

#include "Maths/Maths.hpp"
#include "Resources/Resources.hpp"
#include "Models/Vertex3d.hpp"

namespace acid {
bool ModelSphere::registered = Register("sphere");

std::shared_ptr<ModelSphere> ModelSphere::Create(const Node &node) {
	if (auto resource = Resources::Get()->Find(node)) {
		return std::dynamic_pointer_cast<ModelSphere>(resource);
	}

	auto result = std::make_shared<ModelSphere>(0.0f);
	Resources::Get()->Add(node, std::dynamic_pointer_cast<Resource>(result));
	node >> *result;
	result->Load();
	return result;
}

std::shared_ptr<ModelSphere> ModelSphere::Create(float radius, uint32_t latitudeBands, uint32_t longitudeBands) {
	ModelSphere temp(radius, latitudeBands, longitudeBands, false);
	Node node;
	node << temp;
	return Create(node);
}

ModelSphere::ModelSphere(float radius, uint32_t latitudeBands, uint32_t longitudeBands, bool load) :
	m_radius(radius),
	m_latitudeBands(latitudeBands),
	m_longitudeBands(longitudeBands) {
	if (load) {
		Load();
	}
}

const Node &operator>>(const Node &node, ModelSphere &model) {
	node["latitudeBands"].Get(model.m_latitudeBands);
	node["longitudeBands"].Get(model.m_longitudeBands);
	node["radius"].Get(model.m_radius);
	return node;
}

Node &operator<<(Node &node, const ModelSphere &model) {
	node["latitudeBands"].Set(model.m_latitudeBands);
	node["longitudeBands"].Set(model.m_longitudeBands);
	node["radius"].Set(model.m_radius);
	return node;
}

void ModelSphere::Load() {
	if (m_radius == 0.0f) {
		return;
	}

	std::vector<Vertex3d> vertices;
	std::vector<uint32_t> indices;
	vertices.reserve((m_longitudeBands + 1) * (m_latitudeBands + 1));
	indices.reserve(m_longitudeBands * m_latitudeBands * 6);

	for (uint32_t i = 0; i < m_longitudeBands + 1; i++) {
		auto iDivLong = static_cast<float>(i) / static_cast<float>(m_longitudeBands);
		auto theta = (i == 0 || i == m_longitudeBands) ? 0.0f : iDivLong * 2.0f * Maths::Pi<float>;

		for (uint32_t j = 0; j < m_latitudeBands + 1; j++) {
			auto jDivLat = static_cast<float>(j) / static_cast<float>(m_latitudeBands);
			auto phi = jDivLat * 2.0f * Maths::Pi<float>;

			Vector3f normal(std::cos(phi) * std::sin(theta), std::cos(theta), std::sin(phi) * std::sin(theta));
			auto position = m_radius * normal;
			Vector2f uvs(1.0f - jDivLat, 1.0f - iDivLong);
			vertices.emplace_back(Vertex3d(position, uvs, normal));
		}
	}

	for (uint32_t i = 0; i < m_longitudeBands; i++) {
		for (uint32_t j = 0; j < m_latitudeBands; j++) {
			auto first = j + ((m_latitudeBands + 1) * i);
			auto second = j + ((m_latitudeBands + 1) * (i + 1));

			indices.emplace_back(second + 1);
			indices.emplace_back(first + 1);
			indices.emplace_back(first);
			indices.emplace_back(second);
			indices.emplace_back(second + 1);
			indices.emplace_back(first);
		}
	}

	Initialize(vertices, indices);
}
}
