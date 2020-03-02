#include "RectangleModel.hpp"

#include "Resources/Resources.hpp"
#include "Models/Vertex3d.hpp"

namespace acid {
std::shared_ptr<RectangleModel> RectangleModel::Create(const Node &node) {
	if (auto resource = Resources::Get()->Find<RectangleModel>(node))
		return resource;

	auto result = std::make_shared<RectangleModel>(0.0f, 0.0f);
	Resources::Get()->Add(node, std::dynamic_pointer_cast<Resource>(result));
	node >> *result;
	result->Load();
	return result;
}

std::shared_ptr<RectangleModel> RectangleModel::Create(float min, float max) {
	RectangleModel temp(min, max, false);
	Node node;
	node << temp;
	return Create(node);
}

RectangleModel::RectangleModel(float min, float max, bool load) :
	min(min),
	max(max) {
	if (load) {
		Load();
	}
}

const Node &operator>>(const Node &node, RectangleModel &model) {
	node["min"].Get(model.min);
	node["max"].Get(model.max);
	return node;
}

Node &operator<<(Node &node, const RectangleModel &model) {
	node["min"].Set(model.min);
	node["max"].Set(model.max);
	return node;
}

void RectangleModel::Load() {
	if (min == max)
		return;

	std::vector<Vertex3d> vertices = {
		{{min, min, 0.0f}, {0.0f, 0.0f}, {}},
		{{max, min, 0.0f}, {1.0f, 0.0f}, {}},
		{{max, max, 0.0f}, {1.0f, 1.0f}, {}},
		{{min, max, 0.0f}, {0.0f, 1.0f}, {}}
	};
	static std::vector<uint32_t> indices = {
		0, 1, 2,
		2, 3, 0
	};

	Initialize(vertices, indices);
}
}
