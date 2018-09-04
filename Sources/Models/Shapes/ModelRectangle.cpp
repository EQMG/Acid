#include "ModelRectangle.hpp"

#include "Resources/Resources.hpp"
#include "Models/VertexModel.hpp"

namespace acid
{
	std::shared_ptr<ModelRectangle> ModelRectangle::Resource(const float &min, const float &max)
	{
		auto resource = Resources::Get()->Get(ToFilename(min, max));

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
		auto split = FormatString::Split(data, "_");
		float width = static_cast<float>(atof(split[1].c_str()));
		float height = static_cast<float>(atof(split[2].c_str()));
		return Resource(width, height);
	}

	ModelRectangle::ModelRectangle(const float &min, const float &max) :
		Model()
	{
		std::vector<IVertex *> vertices = {
			new VertexModel(Vector3(min, min, 0.0f), Vector2(0.0f, 0.0f)),
			new VertexModel(Vector3(max, min, 0.0f), Vector2(1.0f, 0.0f)),
			new VertexModel(Vector3(max, max, 0.0f), Vector2(1.0f, 1.0f)),
			new VertexModel(Vector3(min, max, 0.0f), Vector2(0.0f, 1.0f)),
		};
		std::vector<uint32_t> indices = {
			0, 3, 2, 2, 1, 0
		};

		Model::Set(vertices, indices, ToFilename(min, max));
	}

	ModelRectangle::~ModelRectangle()
	{
	}

	std::string ModelRectangle::ToFilename(const float &min, const float &max)
	{
		std::stringstream result;
		result << "Rectangle_" << min << "_" << max;
		return result.str();
	}
}
