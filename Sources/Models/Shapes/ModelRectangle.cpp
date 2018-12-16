#include "ModelRectangle.hpp"

#include "Resources/Resources.hpp"
#include "Models/VertexModel.hpp"

namespace acid
{
	std::shared_ptr<ModelRectangle> ModelRectangle::Resource(const float &min, const float &max)
	{
		auto resource = Resources::Get()->Find(ToFilename(min, max));

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<ModelRectangle>(resource);
		}

		auto result = std::make_shared<ModelRectangle>(min, max);
		Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
		return result;
	}

	std::shared_ptr<ModelRectangle> ModelRectangle::Resource(const std::string &data)
	{
		if (data.empty())
		{
			return nullptr;
		}

		auto split = String::Split(data, "_");
		float width = String::From<float>(split[1]);
		float height = String::From<float>(split[2]);
		return Resource(width, height);
	}

	ModelRectangle::ModelRectangle(const float &min, const float &max) :
		Model()
	{
		auto vertices = std::vector<VertexModel>{
			VertexModel(Vector3(min, min, 0.0f), Vector2(0.0f, 0.0f)),
			VertexModel(Vector3(max, min, 0.0f), Vector2(1.0f, 0.0f)),
			VertexModel(Vector3(max, max, 0.0f), Vector2(1.0f, 1.0f)),
			VertexModel(Vector3(min, max, 0.0f), Vector2(0.0f, 1.0f)),
		};
		auto indices = std::vector<uint32_t>{
			0, 3, 2,
			2, 1, 0
		};

		Model::Initialize(vertices, indices, ToFilename(min, max));
	}

	std::string ModelRectangle::ToFilename(const float &min, const float &max)
	{
		std::stringstream result;
		result << "Rectangle_" << min << "_" << max;
		return result.str();
	}
}
