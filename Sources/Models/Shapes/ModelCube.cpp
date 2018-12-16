#include "ModelCube.hpp"

#include "Resources/Resources.hpp"
#include "Models/VertexModel.hpp"

namespace acid
{
	std::shared_ptr<ModelCube> ModelCube::Resource(const float &width, const float &height, const float &depth)
	{
		auto resource = Resources::Get()->Find(ToFilename(width, height, depth));

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<ModelCube>(resource);
		}

		auto result = std::make_shared<ModelCube>(width, height, depth);
		Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
		return result;
	}

	std::shared_ptr<ModelCube> ModelCube::Resource(const std::string &data)
	{
		if (data.empty())
		{
			return nullptr;
		}

		auto split = String::Split(data, "_");
		float width = String::From<float>(split[1]);
		float height = String::From<float>(split[2]);
		float depth = String::From<float>(split[3]);
		return Resource(width, height, depth);
	}

	ModelCube::ModelCube(const float &width, const float &height, const float &depth) :
		Model()
	{
		auto vertices = std::vector<VertexModel>{
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
		auto indices = std::vector<uint32_t>{
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

		Model::Initialize(vertices, indices, ToFilename(width, height, depth));
	}

	std::string ModelCube::ToFilename(const float &width, const float &height, const float &depth)
	{
		std::stringstream result;
		result << "Cube_" << width << "_" << height << "_" << depth;
		return result.str();
	}
}
