#include "ModelCube.hpp"

#include "Resources/Resources.hpp"
#include "Models/VertexDefault.hpp"

namespace acid
{
std::shared_ptr<ModelCube> ModelCube::Create(const Node &node)
{
	if (auto resource = Resources::Get()->Find(node))
	{
		return std::dynamic_pointer_cast<ModelCube>(resource);
	}

	auto result = std::make_shared<ModelCube>(Vector3f{});
	Resources::Get()->Add(node, std::dynamic_pointer_cast<Resource>(result));
	node >> *result;
	result->Load();
	return result;
}

std::shared_ptr<ModelCube> ModelCube::Create(const Vector3f &extents)
{
	ModelCube temp{extents, false};
	Node node;
	node << temp;
	return Create(node);
}

ModelCube::ModelCube(const Vector3f &extents, bool load) :
	m_extents(extents)
{
	if (load)
	{
		Load();
	}
}

const Node &operator>>(const Node &node, ModelCube &model)
{
	node["extents"].Get(model.m_extents);
	return node;
}

Node &operator<<(Node &node, const ModelCube &model)
{
	node["type"].Set("ModelCube");
	node["extents"].Set(model.m_extents);
	return node;
}

void ModelCube::Load()
{
	if (m_extents == Vector3f::Zero)
	{
		return;
	}

	static std::vector<VertexDefault> vertices{
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
	static std::vector<uint32_t> indices{
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
	{
		vertex.m_position *= m_extents;
	}

	Initialize(vertices, indices);
}
}
