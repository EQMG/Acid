#include "ModelCube.hpp"

#include "Resources/Resources.hpp"
#include "Models/VertexModel.hpp"

namespace acid
{
	std::shared_ptr<ModelCube> ModelCube::Create(const float &width, const float &height, const float &depth)
	{
		auto resource = Resources::Get()->Find(ToName(width, height, depth));

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<ModelCube>(resource);
		}

		auto result = std::make_shared<ModelCube>(width, height, depth);
		Resources::Get()->Add(std::dynamic_pointer_cast<Resource>(result));
		return result;
	}

	std::shared_ptr<ModelCube> ModelCube::Create(const std::string &data)
	{
		if (data.empty())
		{
			return nullptr;
		}

		auto split = String::Split(data, "_");
		auto width = String::From<float>(split[1]);
		auto height = String::From<float>(split[2]);
		auto depth = String::From<float>(split[3]);
		return Create(width, height, depth);
	}

	ModelCube::ModelCube(const float &width, const float &height, const float &depth) :
		Model()
	{
		std::vector<VertexModel> vertices = {
			VertexModel(Vector3(-0.5f, -0.5f, 0.5f), Vector2(0.375f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)),
			VertexModel(Vector3(-0.5f, 0.5f, 0.5f), Vector2(0.625f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)),
			VertexModel(Vector3(-0.5f, -0.5f, -0.5f), Vector2(0.375f, 0.75f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)),
			VertexModel(Vector3(-0.5f, 0.5f, -0.5f), Vector2(0.625f, 0.75f), Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f)),
			VertexModel(Vector3(0.5f, -0.5f, 0.5f), Vector2(0.375f, 0.25f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)),
			VertexModel(Vector3(0.5f, 0.5f, 0.5f), Vector2(0.625f, 0.25f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f)),
			VertexModel(Vector3(0.5f, -0.5f, -0.5f), Vector2(0.375f, 0.5f), Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f)),
			VertexModel(Vector3(0.5f, 0.5f, -0.5f), Vector2(0.625f, 0.5f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)),
			VertexModel(Vector3(-0.5f, -0.5f, -0.5f), Vector2(0.375f, 0.75f), Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f)),
			VertexModel(Vector3(0.5f, -0.5f, -0.5f), Vector2(0.375f, 0.5f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)),
			VertexModel(Vector3(-0.5f, -0.5f, 0.5f), Vector2(0.375f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f)),
			VertexModel(Vector3(0.5f, -0.5f, 0.5f), Vector2(0.375f, 0.25f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f)),
			VertexModel(Vector3(0.5f, -0.5f, -0.5f), Vector2(0.375f, 0.5f), Vector3(0.0f, -1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f)),
			VertexModel(Vector3(-0.5f, -0.5f, 0.5f), Vector2(0.125f, 0.25f), Vector3(0.0f, -1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f)),
			VertexModel(Vector3(-0.5f, -0.5f, -0.5f), Vector2(0.125f, 0.5f), Vector3(0.0f, -1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f)),
			VertexModel(Vector3(-0.5f, 0.5f, -0.5f), Vector2(0.875f, 0.5f), Vector3(0.0f, 1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f)),
			VertexModel(Vector3(0.5f, 0.5f, 0.5f), Vector2(0.625f, 0.25f), Vector3(0.0f, 1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f)),
			VertexModel(Vector3(0.5f, 0.5f, -0.5f), Vector2(0.625f, 0.5f), Vector3(0.0f, 1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f)),
			VertexModel(Vector3(-0.5f, 0.5f, -0.5f), Vector2(0.625f, 0.75f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)),
			VertexModel(Vector3(0.5f, 0.5f, -0.5f), Vector2(0.625f, 0.5f), Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f)),
			VertexModel(Vector3(0.5f, 0.5f, 0.5f), Vector2(0.625f, 0.25f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)),
			VertexModel(Vector3(-0.5f, 0.5f, 0.5f), Vector2(0.625f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f)),
			VertexModel(Vector3(0.5f, -0.5f, 0.5f), Vector2(0.375f, 0.25f), Vector3(0.0f, -1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f)),
			VertexModel(Vector3(-0.5f, 0.5f, 0.5f), Vector2(0.875f, 0.25f), Vector3(0.0f, 1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f)),
		};
		std::vector<uint32_t> indices = {
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
			vertex.SetPosition(vertex.GetPosition() * Vector3(width, height, depth));
		}

		Model::Initialize(vertices, indices, ToName(width, height, depth));
	}

	std::string ModelCube::ToName(const float &width, const float &height, const float &depth)
	{
		std::stringstream result;
		result << "Cube_" << width << "_" << height << "_" << depth;
		return result.str();
	}
}
