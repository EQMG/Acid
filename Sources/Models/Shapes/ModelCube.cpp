#include "ModelCube.hpp"

#include "Resources/Resources.hpp"
#include "Models/VertexModel.hpp"

namespace acid
{
std::shared_ptr<ModelCube> ModelCube::Create(const Metadata &metadata)
{
	auto resource = Resources::Get()->Find(metadata);

	if (resource != nullptr)
	{
		return std::dynamic_pointer_cast<ModelCube>(resource);
	}

	auto result = std::make_shared<ModelCube>(0.0f, 0.0f, 0.0f);
	Resources::Get()->Add(metadata, std::dynamic_pointer_cast<Resource>(result));
	result->Decode(metadata);
	result->Load();
	return result;
}

std::shared_ptr<ModelCube> ModelCube::Create(const float &width, const float &height, const float &depth)
{
	auto temp = ModelCube(width, height, depth, false);
	Metadata metadata = Metadata();
	temp.Encode(metadata);
	return Create(metadata);
}

ModelCube::ModelCube(const float &width, const float &height, const float &depth, const bool &load) :
	m_width(width),
	m_height(height),
	m_depth(depth)
{
	if (load)
	{
		ModelCube::Load();
	}
}

void ModelCube::Load()
{
	if (m_width == 0.0f && m_height == 0.0f && m_depth == 0.0f)
	{
		return;
	}

	static std::vector<VertexModel> vertices = { VertexModel(Vector3(-0.5f, -0.5f, 0.5f), Vector2(0.375f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f)),
		VertexModel(Vector3(-0.5f, 0.5f, 0.5f), Vector2(0.625f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f)),
		VertexModel(Vector3(-0.5f, -0.5f, -0.5f), Vector2(0.375f, 0.75f), Vector3(-1.0f, 0.0f, 0.0f)),
		VertexModel(Vector3(-0.5f, 0.5f, -0.5f), Vector2(0.625f, 0.75f), Vector3(0.0f, 0.0f, -1.0f)),
		VertexModel(Vector3(0.5f, -0.5f, 0.5f), Vector2(0.375f, 0.25f), Vector3(1.0f, 0.0f, 0.0f)),
		VertexModel(Vector3(0.5f, 0.5f, 0.5f), Vector2(0.625f, 0.25f), Vector3(0.0f, 0.0f, 1.0f)),
		VertexModel(Vector3(0.5f, -0.5f, -0.5f), Vector2(0.375f, 0.5f), Vector3(0.0f, 0.0f, -1.0f)),
		VertexModel(Vector3(0.5f, 0.5f, -0.5f), Vector2(0.625f, 0.5f), Vector3(1.0f, 0.0f, 0.0f)),
		VertexModel(Vector3(-0.5f, -0.5f, -0.5f), Vector2(0.375f, 0.75f), Vector3(0.0f, 0.0f, -1.0f)),
		VertexModel(Vector3(0.5f, -0.5f, -0.5f), Vector2(0.375f, 0.5f), Vector3(1.0f, 0.0f, 0.0f)),
		VertexModel(Vector3(-0.5f, -0.5f, 0.5f), Vector2(0.375f, 0.0f), Vector3(0.0f, 0.0f, 1.0f)),
		VertexModel(Vector3(0.5f, -0.5f, 0.5f), Vector2(0.375f, 0.25f), Vector3(0.0f, 0.0f, 1.0f)),
		VertexModel(Vector3(0.5f, -0.5f, -0.5f), Vector2(0.375f, 0.5f), Vector3(0.0f, -1.0f, 0.0f)),
		VertexModel(Vector3(-0.5f, -0.5f, 0.5f), Vector2(0.125f, 0.25f), Vector3(0.0f, -1.0f, 0.0f)),
		VertexModel(Vector3(-0.5f, -0.5f, -0.5f), Vector2(0.125f, 0.5f), Vector3(0.0f, -1.0f, 0.0f)),
		VertexModel(Vector3(-0.5f, 0.5f, -0.5f), Vector2(0.875f, 0.5f), Vector3(0.0f, 1.0f, 0.0f)),
		VertexModel(Vector3(0.5f, 0.5f, 0.5f), Vector2(0.625f, 0.25f), Vector3(0.0f, 1.0f, 0.0f)),
		VertexModel(Vector3(0.5f, 0.5f, -0.5f), Vector2(0.625f, 0.5f), Vector3(0.0f, 1.0f, 0.0f)),
		VertexModel(Vector3(-0.5f, 0.5f, -0.5f), Vector2(0.625f, 0.75f), Vector3(-1.0f, 0.0f, 0.0f)),
		VertexModel(Vector3(0.5f, 0.5f, -0.5f), Vector2(0.625f, 0.5f), Vector3(0.0f, 0.0f, -1.0f)),
		VertexModel(Vector3(0.5f, 0.5f, 0.5f), Vector2(0.625f, 0.25f), Vector3(1.0f, 0.0f, 0.0f)),
		VertexModel(Vector3(-0.5f, 0.5f, 0.5f), Vector2(0.625f, 0.0f), Vector3(0.0f, 0.0f, 1.0f)),
		VertexModel(Vector3(0.5f, -0.5f, 0.5f), Vector2(0.375f, 0.25f), Vector3(0.0f, -1.0f, 0.0f)),
		VertexModel(Vector3(-0.5f, 0.5f, 0.5f), Vector2(0.875f, 0.25f), Vector3(0.0f, 1.0f, 0.0f)), };
	static std::vector<uint32_t> indices = { 1, 2, 0, // Front
		3, 6, 8, 7, 4, 9, // Back
		5, 10, 11, 12, 13, 14, // Top
		15, 16, 17, 1, 18, 2, // Bottom
		3, 19, 6, 7, 20, 4, // Left
		5, 21, 10, 12, 22, 13, // Right
		15, 23, 16, };

	for (auto &vertex : vertices)
	{
		vertex.SetPosition(vertex.GetPosition() * Vector3(m_width, m_height, m_depth));
	}

	Initialize(vertices, indices);
}

void ModelCube::Decode(const Metadata &metadata)
{
	metadata.GetChild("Width", m_width);
	metadata.GetChild("Height", m_height);
	metadata.GetChild("Depth", m_depth);
}

void ModelCube::Encode(Metadata &metadata) const
{
	metadata.SetChild<std::string>("Type", "ModelCube");
	metadata.SetChild("Width", m_width);
	metadata.SetChild("Height", m_height);
	metadata.SetChild("Depth", m_depth);
}
}
