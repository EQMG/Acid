#include "SphereModel.hpp"

#include "Maths/Maths.hpp"
#include "Resources/Resources.hpp"
#include "Models/Vertex3d.hpp"

namespace acid {
const bool SphereModel::Registered = Register("sphere");

std::shared_ptr<SphereModel> SphereModel::Create(const Node &node) {
	if (auto resource = Resources::Get()->Find<SphereModel>(node))
		return resource;

	auto result = std::make_shared<SphereModel>(0.0f);
	Resources::Get()->Add(node, std::dynamic_pointer_cast<Resource>(result));
	node >> *result;
	result->Load();
	return result;
}

std::shared_ptr<SphereModel> SphereModel::Create(float radius, uint32_t latitudeBands, uint32_t longitudeBands) {
	SphereModel temp(radius, latitudeBands, longitudeBands, false);
	Node node;
	node << temp;
	return Create(node);
}

SphereModel::SphereModel(float radius, uint32_t latitudeBands, uint32_t longitudeBands, bool load) :
	radius(radius),
	latitudeBands(latitudeBands),
	longitudeBands(longitudeBands) {
	if (load) {
		Load();
	}
}

const Node &operator>>(const Node &node, SphereModel &model) {
	node["latitudeBands"].Get(model.latitudeBands);
	node["longitudeBands"].Get(model.longitudeBands);
	node["radius"].Get(model.radius);
	return node;
}

Node &operator<<(Node &node, const SphereModel &model) {
	node["latitudeBands"].Set(model.latitudeBands);
	node["longitudeBands"].Set(model.longitudeBands);
	node["radius"].Set(model.radius);
	return node;
}

void SphereModel::Load() {
	if (radius == 0.0f)
		return;

	std::vector<Vertex3d> vertices;
	std::vector<uint32_t> indices;
	vertices.reserve((longitudeBands + 1) * (latitudeBands + 1));
	indices.reserve(longitudeBands * latitudeBands * 6);

	for (uint32_t i = 0; i < longitudeBands + 1; i++) {
		auto iDivLong = static_cast<float>(i) / static_cast<float>(longitudeBands);
		auto theta = (i == 0 || i == longitudeBands) ? 0.0f : iDivLong * 2.0f * Maths::PI<float>;

		for (uint32_t j = 0; j < latitudeBands + 1; j++) {
			auto jDivLat = static_cast<float>(j) / static_cast<float>(latitudeBands);
			auto phi = jDivLat * 2.0f * Maths::PI<float>;

			Vector3f normal(std::cos(phi) * std::sin(theta), std::cos(theta), std::sin(phi) * std::sin(theta));
			auto position = radius * normal;
			Vector2f uvs(1.0f - jDivLat, 1.0f - iDivLong);
			vertices.emplace_back(Vertex3d(position, uvs, normal));
		}
	}

	for (uint32_t i = 0; i < longitudeBands; i++) {
		for (uint32_t j = 0; j < latitudeBands; j++) {
			auto first = j + ((latitudeBands + 1) * i);
			auto second = j + ((latitudeBands + 1) * (i + 1));

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
