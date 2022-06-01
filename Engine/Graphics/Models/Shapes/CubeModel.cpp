#include "CubeModel.hpp"

#include "Resources/Resources.hpp"
#include "Models/Vertex3d.hpp"

namespace acid {
std::shared_ptr<CubeModel> CubeModel::Create(const Node &node) {
	if (auto resource = Resources::Get()->Find<CubeModel>(node))
		return resource;

	auto result = std::make_shared<CubeModel>(Vector3f());
	Resources::Get()->Add(node, std::dynamic_pointer_cast<Resource>(result));
	node >> *result;
	result->Load();
	return result;
}

std::shared_ptr<CubeModel> CubeModel::Create(const Vector3f &extents) {
	CubeModel temp(extents, false);
	Node node;
	node << temp;
	return Create(node);
}

CubeModel::CubeModel(const Vector3f &extents, bool load) :
	extents(extents) {
	if (load) {
		Load();
	}
}

const Node &operator>>(const Node &node, CubeModel &model) {
	node["extents"].Get(model.extents);
	return node;
}

Node &operator<<(Node &node, const CubeModel &model) {
	node["extents"].Set(model.extents);
	return node;
}

void CubeModel::Load() {
	if (extents == Vector3f::Zero)
		return;

	std::vector<Vertex3d> vertices = {
		{{-0.5f, -0.5f, 0.5f}, {0.375f, 1.0f}, {-1.0f, 0.0f, 0.0f}},
		{{-0.5f, 0.5f, 0.5f}, {0.625f, 1.0f}, {-1.0f, 0.0f, 0.0f}},
		{{-0.5f, -0.5f, -0.5f}, {0.375f, 0.75f}, {-1.0f, 0.0f, 0.0f}},
		{{-0.5f, 0.5f, -0.5f}, {0.625f, 0.75f}, {0.0f, 0.0f, -1.0f}},
		{{0.5f, -0.5f, 0.5f}, {0.375f, 0.25f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, 0.5f, 0.5f}, {0.625f, 0.25f}, {0.0f, 0.0f, 1.0f}},
		{{0.5f, -0.5f, -0.5f}, {0.375f, 0.5f}, {0.0f, 0.0f, -1.0f}},
		{{0.5f, 0.5f, -0.5f}, {0.625f, 0.5f}, {1.0f, 0.0f, 0.0f}},
		{{-0.5f, -0.5f, -0.5f}, {0.375f, 0.75f}, {0.0f, 0.0f, -1.0f}},
		{{0.5f, -0.5f, -0.5f}, {0.375f, 0.5f}, {1.0f, 0.0f, 0.0f}},
		{{-0.5f, -0.5f, 0.5f}, {0.375f, 0.0f}, {0.0f, 0.0f, 1.0f}},
		{{0.5f, -0.5f, 0.5f}, {0.375f, 0.25f}, {0.0f, 0.0f, 1.0f}},
		{{0.5f, -0.5f, -0.5f}, {0.375f, 0.5f}, {0.0f, -1.0f, 0.0f}},
		{{-0.5f, -0.5f, 0.5f}, {0.125f, 0.25f}, {0.0f, -1.0f, 0.0f}},
		{{-0.5f, -0.5f, -0.5f}, {0.125f, 0.5f}, {0.0f, -1.0f, 0.0f}},
		{{-0.5f, 0.5f, -0.5f}, {0.875f, 0.5f}, {0.0f, 1.0f, 0.0f}},
		{{0.5f, 0.5f, 0.5f}, {0.625f, 0.25f}, {0.0f, 1.0f, 0.0f}},
		{{0.5f, 0.5f, -0.5f}, {0.625f, 0.5f}, {0.0f, 1.0f, 0.0f}},
		{{-0.5f, 0.5f, -0.5f}, {0.625f, 0.75f}, {-1.0f, 0.0f, 0.0f}},
		{{0.5f, 0.5f, -0.5f}, {0.625f, 0.5f}, {0.0f, 0.0f, -1.0f}},
		{{0.5f, 0.5f, 0.5f}, {0.625f, 0.25f}, {1.0f, 0.0f, 0.0f}},
		{{-0.5f, 0.5f, 0.5f}, {0.625f, 0.0f}, {0.0f, 0.0f, 1.0f}},
		{{0.5f, -0.5f, 0.5f}, {0.375f, 0.25f}, {0.0f, -1.0f, 0.0f}},
		{{-0.5f, 0.5f, 0.5f}, {0.875f, 0.25f}, {0.0f, 1.0f, 0.0f}}
	};
	static std::vector<uint32_t> indices = {
		1, 2, 0, // Front
		3, 6, 8,
		7, 4, 9, // Back
		5, 10, 11,
		12, 13, 14, // Top
		15, 16, 17,
		1, 18, 2, // Bottom
		3, 19, 6,
		7, 20, 4, // Left
		5, 21, 10,
		12, 22, 13, // Right
		15, 23, 16,
	};

	for (auto &vertex : vertices)
		vertex.position *= extents;

	Initialize(vertices, indices);
}
}
