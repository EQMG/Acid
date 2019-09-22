#include "ModelDisk.hpp"

#include "Maths/Maths.hpp"
#include "Resources/Resources.hpp"
#include "Models/VertexDefault.hpp"

namespace acid {
bool ModelDisk::registered = Register("disk");

std::shared_ptr<ModelDisk> ModelDisk::Create(const Node &node) {
	if (auto resource = Resources::Get()->Find(node)) {
		return std::dynamic_pointer_cast<ModelDisk>(resource);
	}

	auto result = std::make_shared<ModelDisk>(0.0f, 0.0f);
	Resources::Get()->Add(node, std::dynamic_pointer_cast<Resource>(result));
	node >> *result;
	result->Load();
	return result;
}

std::shared_ptr<ModelDisk> ModelDisk::Create(float innerRadius, float outerRadius, uint32_t slices, uint32_t loops) {
	ModelDisk temp(innerRadius, outerRadius, slices, loops, false);
	Node node;
	node << temp;
	return Create(node);
}

ModelDisk::ModelDisk(float innerRadius, float outerRadius, uint32_t slices, uint32_t loops, bool load) :
	m_innerRadius(innerRadius),
	m_outerRadius(outerRadius),
	m_slices(slices),
	m_loops(loops) {
	if (load) {
		Load();
	}
}

const Node &operator>>(const Node &node, ModelDisk &model) {
	node["innerRadius"].Get(model.m_innerRadius);
	node["outerRadius"].Get(model.m_outerRadius);
	node["slices"].Get(model.m_slices);
	node["loops"].Get(model.m_loops);
	return node;
}

Node &operator<<(Node &node, const ModelDisk &model) {
	node["type"].Set("ModelDisk");
	node["innerRadius"].Set(model.m_innerRadius);
	node["outerRadius"].Set(model.m_outerRadius);
	node["slices"].Set(model.m_slices);
	node["loops"].Set(model.m_loops);
	return node;
}

void ModelDisk::Load() {
	if (m_innerRadius == 0.0f && m_outerRadius == 0.0f) {
		return;
	}

	std::vector<VertexDefault> vertices;
	std::vector<uint32_t> indices;
	vertices.reserve(m_slices * (m_loops + 1));
	indices.reserve(m_slices * m_loops * 6);

	for (uint32_t i = 0; i < m_slices; i++) {
		auto iDivSlices = static_cast<float>(i) / static_cast<float>(m_slices);
		auto alpha = iDivSlices * 2.0f * Maths::Pi<float>;
		auto xDir = std::cos(alpha);
		auto yDir = std::sin(alpha);

		for (uint32_t j = 0; j < m_loops + 1; j++) {
			auto jDivLoops = static_cast<float>(j) / static_cast<float>(m_loops);
			auto radius = m_innerRadius + jDivLoops * (m_outerRadius - m_innerRadius);

			Vector3f position(radius * xDir, 0.0f, radius * yDir);
			Vector2f uvs(1.0f - iDivSlices, 1.0f - jDivLoops);
			Vector3f normal(0.0f, 1.0f, 0.0f);
			vertices.emplace_back(VertexDefault(position, uvs, normal));
		}
	}

	for (uint32_t i = 0; i < m_slices; i++) {
		for (uint32_t j = 0; j < m_loops; j++) {
			auto first = i * (m_loops + 1) + j;
			auto second = (first + m_loops + 1) % (m_slices * (m_loops + 1));

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
