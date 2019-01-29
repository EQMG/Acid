#include "ModelRectangle.hpp"

#include "Resources/Resources.hpp"
#include "Models/VertexModel.hpp"

namespace acid
{
	std::shared_ptr<ModelRectangle> ModelRectangle::Create(const float &min, const float &max)
	{
		std::shared_ptr<Resource> resource = nullptr; // FIXME: Resources::Get()->Find(ToName(min, max));

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<ModelRectangle>(resource);
		}

		auto result = std::make_shared<ModelRectangle>(min, max);
		// Resources::Get()->Add(std::dynamic_pointer_cast<Resource>(result));
		return result;
	}

	ModelRectangle::ModelRectangle(const float &min, const float &max) :
		Model(),
		m_min(min),
		m_max(max)
	{
		std::vector<VertexModel> vertices = {
			VertexModel(Vector3(min, min, 0.0f), Vector2(0.0f, 0.0f)),
			VertexModel(Vector3(max, min, 0.0f), Vector2(1.0f, 0.0f)),
			VertexModel(Vector3(max, max, 0.0f), Vector2(1.0f, 1.0f)),
			VertexModel(Vector3(min, max, 0.0f), Vector2(0.0f, 1.0f)),
		};
		std::vector<uint32_t> indices = {
			0, 3, 2,
			2, 1, 0
		};

		Model::Initialize(vertices, indices);
	}

	/*void ModelRectangle::Decode(const Metadata &metadata)
	{
		m_min = metadata.GetChild<float>("Min");
		m_max = metadata.GetChild<float>("Max");
	}*/

	void ModelRectangle::Encode(Metadata &metadata) const
	{
		metadata.SetChild<std::string>("Type", "ModelRectangle");
		metadata.SetChild<float>("Min", m_min);
		metadata.SetChild<float>("Max", m_max);
	}
}
