#include "ModelRectangle.hpp"

#include "Resources/Resources.hpp"
#include "Models/VertexModel.hpp"

namespace acid
{
	std::shared_ptr<ModelRectangle> ModelRectangle::Create(const float &min, const float &max)
	{
		auto resource = Resources::Get()->Find(ToName(min, max));

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<ModelRectangle>(resource);
		}

		auto result = std::make_shared<ModelRectangle>(min, max);
		Resources::Get()->Add(std::dynamic_pointer_cast<Resource>(result));
		return result;
	}

	std::shared_ptr<ModelRectangle> ModelRectangle::Create(const std::string &data)
	{
		if (data.empty())
		{
			return nullptr;
		}

		auto split = String::Split(data, "_");
		auto width = String::From<float>(split[1]);
		auto height = String::From<float>(split[2]);
		return Create(width, height);
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

		Model::Initialize(vertices, indices, ToName(min, max));
	}

	std::string ModelRectangle::ToName(const float &min, const float &max)
	{
		std::stringstream result;
		result << "Rectangle_" << min << "_" << max;
		return result.str();
	}
}
