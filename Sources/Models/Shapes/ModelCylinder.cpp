#include "ModelCylinder.hpp"

#include "Maths/Maths.hpp"
#include "Resources/Resources.hpp"
#include "Models/Vertex3d.hpp"

namespace acid {
std::shared_ptr<ModelCylinder> ModelCylinder::Create(const Node &node) {
	if (auto resource = Resources::Get()->Find<ModelCylinder>(node))
		return resource;

	auto result = std::make_shared<ModelCylinder>(0.0f, 0.0f);
	Resources::Get()->Add(node, std::dynamic_pointer_cast<Resource>(result));
	node >> *result;
	result->Load();
	return result;
}

std::shared_ptr<ModelCylinder> ModelCylinder::Create(float radiusBase, float radiusTop, float height, uint32_t slices, uint32_t stacks) {
	ModelCylinder temp(radiusBase, radiusTop, height, slices, stacks, false);
	Node node;
	node << temp;
	return Create(node);
}

ModelCylinder::ModelCylinder(float radiusBase, float radiusTop, float height, uint32_t slices, uint32_t stacks, bool load) :
	radiusBase(radiusBase),
	radiusTop(radiusTop),
	height(height),
	slices(slices),
	stacks(stacks) {
	if (load) {
		Load();
	}
}

const Node &operator>>(const Node &node, ModelCylinder &model) {
	node["radiusBase"].Get(model.radiusBase);
	node["radiusTop"].Get(model.radiusTop);
	node["height"].Get(model.height);
	node["slices"].Get(model.slices);
	node["stacks"].Get(model.stacks);
	return node;
}

Node &operator<<(Node &node, const ModelCylinder &model) {
	node["radiusBase"].Set(model.radiusBase);
	node["radiusTop"].Set(model.radiusTop);
	node["height"].Set(model.height);
	node["slices"].Set(model.slices);
	node["stacks"].Set(model.stacks);
	return node;
}

void ModelCylinder::Load() {
	if (radiusBase == 0.0f && radiusTop == 0.0f) {
		return;
	}

	std::vector<Vertex3d> vertices;
	std::vector<uint32_t> indices;
	vertices.reserve((slices + 1) * (stacks + 1));
	indices.reserve(slices * stacks * 6);

	for (uint32_t i = 0; i < slices + 1; i++) {
		auto iDivSlices = static_cast<float>(i) / static_cast<float>(slices);
		auto alpha = (i == 0 || i == slices) ? 0.0f : iDivSlices * 2.0f * Maths::PI<float>;
		auto xDir = std::cos(alpha);
		auto zDir = std::sin(alpha);

		for (uint32_t j = 0; j < stacks + 1; j++) {
			auto jDivStacks = static_cast<float>(j) / static_cast<float>(stacks);
			auto radius = radiusBase * (1.0f - jDivStacks) + radiusTop * jDivStacks;

			Vector3f position(xDir * radius, jDivStacks * height - (height / 2.0f), zDir * radius);
			Vector2f uvs(1.0f - iDivSlices, 1.0f - jDivStacks);
			Vector3f normal(xDir, 0.0f, zDir);
			vertices.emplace_back(Vertex3d(position, uvs, normal));
		}
	}

	for (uint32_t i = 0; i < slices; i++) {
		for (uint32_t j = 0; j < stacks; j++) {
			auto first = j + ((stacks + 1) * i);
			auto second = j + ((stacks + 1) * (i + 1));

			indices.emplace_back(first + 1);
			indices.emplace_back(second + 1);
			indices.emplace_back(first);
			indices.emplace_back(second + 1);
			indices.emplace_back(second);
			indices.emplace_back(first);
		}
	}

	Initialize(vertices, indices);
}
}
