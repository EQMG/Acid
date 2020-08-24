#include "DiskModel.hpp"

#include "Maths/Maths.hpp"
#include "Resources/Resources.hpp"
#include "Models/Vertex3d.hpp"

namespace acid {
const bool DiskModel::Registered = Register("disk");

std::shared_ptr<DiskModel> DiskModel::Create(const Node &node) {
	if (auto resource = Resources::Get()->Find<DiskModel>(node))
		return resource;

	auto result = std::make_shared<DiskModel>(0.0f, 0.0f);
	Resources::Get()->Add(node, std::dynamic_pointer_cast<Resource>(result));
	node >> *result;
	result->Load();
	return result;
}

std::shared_ptr<DiskModel> DiskModel::Create(float innerRadius, float outerRadius, uint32_t slices, uint32_t loops) {
	DiskModel temp(innerRadius, outerRadius, slices, loops, false);
	Node node;
	node << temp;
	return Create(node);
}

DiskModel::DiskModel(float innerRadius, float outerRadius, uint32_t slices, uint32_t loops, bool load) :
	innerRadius(innerRadius),
	outerRadius(outerRadius),
	slices(slices),
	loops(loops) {
	if (load) {
		Load();
	}
}

const Node &operator>>(const Node &node, DiskModel &model) {
	node["innerRadius"].Get(model.innerRadius);
	node["outerRadius"].Get(model.outerRadius);
	node["slices"].Get(model.slices);
	node["loops"].Get(model.loops);
	return node;
}

Node &operator<<(Node &node, const DiskModel &model) {
	node["innerRadius"].Set(model.innerRadius);
	node["outerRadius"].Set(model.outerRadius);
	node["slices"].Set(model.slices);
	node["loops"].Set(model.loops);
	return node;
}

void DiskModel::Load() {
	if (innerRadius == 0.0f && outerRadius == 0.0f)
		return;

	std::vector<Vertex3d> vertices;
	std::vector<uint32_t> indices;
	vertices.reserve(slices * (loops + 1));
	indices.reserve(slices * loops * 6);

	for (uint32_t i = 0; i < slices; i++) {
		auto iDivSlices = static_cast<float>(i) / static_cast<float>(slices);
		auto alpha = iDivSlices * 2.0f * Maths::PI<float>;
		auto xDir = std::cos(alpha);
		auto yDir = std::sin(alpha);

		for (uint32_t j = 0; j < loops + 1; j++) {
			auto jDivLoops = static_cast<float>(j) / static_cast<float>(loops);
			auto radius = innerRadius + jDivLoops * (outerRadius - innerRadius);

			Vector3f position(radius * xDir, 0.0f, radius * yDir);
			Vector2f uvs(1.0f - iDivSlices, 1.0f - jDivLoops);
			Vector3f normal(0.0f, 1.0f, 0.0f);
			vertices.emplace_back(Vertex3d(position, uvs, normal));
		}
	}

	for (uint32_t i = 0; i < slices; i++) {
		for (uint32_t j = 0; j < loops; j++) {
			auto first = i * (loops + 1) + j;
			auto second = (first + loops + 1) % (slices * (loops + 1));

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
