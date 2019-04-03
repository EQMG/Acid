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

	auto result = std::make_shared<ModelCube>(Vector3f::Zero);
	Resources::Get()->Add(metadata, std::dynamic_pointer_cast<Resource>(result));
	result->Decode(metadata);
	result->Load();
	return result;
}

std::shared_ptr<ModelCube> ModelCube::Create(const Vector3f &extents)
{
	auto temp = ModelCube(extents, false);
	Metadata metadata = Metadata();
	temp.Encode(metadata);
	return Create(metadata);
}

ModelCube::ModelCube(const Vector3f &extents, const bool &load) :
	m_extents(extents)
{
	if (load)
	{
		ModelCube::Load();
	}
}

void ModelCube::Load()
{
	if (m_extents == Vector3f::Zero)
	{
		return;
	}

	static std::vector<VertexModel> vertices = {
		VertexModel(Vector3f(-0.5f, -0.5f, 0.5f), Vector2f(0.375f, 1.0f), Vector3f(-1.0f, 0.0f, 0.0f)),
		VertexModel(Vector3f(-0.5f, 0.5f, 0.5f), Vector2f(0.625f, 1.0f), Vector3f(-1.0f, 0.0f, 0.0f)),
		VertexModel(Vector3f(-0.5f, -0.5f, -0.5f), Vector2f(0.375f, 0.75f), Vector3f(-1.0f, 0.0f, 0.0f)),
		VertexModel(Vector3f(-0.5f, 0.5f, -0.5f), Vector2f(0.625f, 0.75f), Vector3f(0.0f, 0.0f, -1.0f)),
		VertexModel(Vector3f(0.5f, -0.5f, 0.5f), Vector2f(0.375f, 0.25f), Vector3f(1.0f, 0.0f, 0.0f)),
		VertexModel(Vector3f(0.5f, 0.5f, 0.5f), Vector2f(0.625f, 0.25f), Vector3f(0.0f, 0.0f, 1.0f)),
		VertexModel(Vector3f(0.5f, -0.5f, -0.5f), Vector2f(0.375f, 0.5f), Vector3f(0.0f, 0.0f, -1.0f)),
		VertexModel(Vector3f(0.5f, 0.5f, -0.5f), Vector2f(0.625f, 0.5f), Vector3f(1.0f, 0.0f, 0.0f)),
		VertexModel(Vector3f(-0.5f, -0.5f, -0.5f), Vector2f(0.375f, 0.75f), Vector3f(0.0f, 0.0f, -1.0f)),
		VertexModel(Vector3f(0.5f, -0.5f, -0.5f), Vector2f(0.375f, 0.5f), Vector3f(1.0f, 0.0f, 0.0f)),
		VertexModel(Vector3f(-0.5f, -0.5f, 0.5f), Vector2f(0.375f, 0.0f), Vector3f(0.0f, 0.0f, 1.0f)),
		VertexModel(Vector3f(0.5f, -0.5f, 0.5f), Vector2f(0.375f, 0.25f), Vector3f(0.0f, 0.0f, 1.0f)),
		VertexModel(Vector3f(0.5f, -0.5f, -0.5f), Vector2f(0.375f, 0.5f), Vector3f(0.0f, -1.0f, 0.0f)),
		VertexModel(Vector3f(-0.5f, -0.5f, 0.5f), Vector2f(0.125f, 0.25f), Vector3f(0.0f, -1.0f, 0.0f)),
		VertexModel(Vector3f(-0.5f, -0.5f, -0.5f), Vector2f(0.125f, 0.5f), Vector3f(0.0f, -1.0f, 0.0f)),
		VertexModel(Vector3f(-0.5f, 0.5f, -0.5f), Vector2f(0.875f, 0.5f), Vector3f(0.0f, 1.0f, 0.0f)),
		VertexModel(Vector3f(0.5f, 0.5f, 0.5f), Vector2f(0.625f, 0.25f), Vector3f(0.0f, 1.0f, 0.0f)),
		VertexModel(Vector3f(0.5f, 0.5f, -0.5f), Vector2f(0.625f, 0.5f), Vector3f(0.0f, 1.0f, 0.0f)),
		VertexModel(Vector3f(-0.5f, 0.5f, -0.5f), Vector2f(0.625f, 0.75f), Vector3f(-1.0f, 0.0f, 0.0f)),
		VertexModel(Vector3f(0.5f, 0.5f, -0.5f), Vector2f(0.625f, 0.5f), Vector3f(0.0f, 0.0f, -1.0f)),
		VertexModel(Vector3f(0.5f, 0.5f, 0.5f), Vector2f(0.625f, 0.25f), Vector3f(1.0f, 0.0f, 0.0f)),
		VertexModel(Vector3f(-0.5f, 0.5f, 0.5f), Vector2f(0.625f, 0.0f), Vector3f(0.0f, 0.0f, 1.0f)),
		VertexModel(Vector3f(0.5f, -0.5f, 0.5f), Vector2f(0.375f, 0.25f), Vector3f(0.0f, -1.0f, 0.0f)),
		VertexModel(Vector3f(-0.5f, 0.5f, 0.5f), Vector2f(0.875f, 0.25f), Vector3f(0.0f, 1.0f, 0.0f)),
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
	{
		vertex.SetPosition(vertex.GetPosition() * m_extents);
	}

	Initialize(vertices, indices);
}

void ModelCube::Decode(const Metadata &metadata)
{
	metadata.GetChild("Extents", m_extents);
}

void ModelCube::Encode(Metadata &metadata) const
{
	metadata.SetChild<std::string>("Type", "ModelCube");
	metadata.SetChild("Extents", m_extents);
}
}
