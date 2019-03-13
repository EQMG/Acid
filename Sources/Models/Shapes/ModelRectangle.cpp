#include "ModelRectangle.hpp"

#include "Models/VertexModel.hpp"
#include "Resources/Resources.hpp"

namespace acid
{
std::shared_ptr<ModelRectangle> ModelRectangle::Create(const Metadata& metadata)
{
	auto resource = Resources::Get()->Find(metadata);

	if(resource != nullptr)
		{
			return std::dynamic_pointer_cast<ModelRectangle>(resource);
		}

	auto result = std::make_shared<ModelRectangle>(0.0f, 0.0f);
	Resources::Get()->Add(metadata, std::dynamic_pointer_cast<Resource>(result));
	result->Decode(metadata);
	result->Load();
	return result;
}

std::shared_ptr<ModelRectangle> ModelRectangle::Create(const float& min, const float& max)
{
	auto temp = ModelRectangle(min, max, false);
	Metadata metadata = Metadata();
	temp.Encode(metadata);
	return Create(metadata);
}

ModelRectangle::ModelRectangle(const float& min, const float& max, const bool& load) : m_min(min), m_max(max)
{
	if(load)
		{
			ModelRectangle::Load();
		}
}

void ModelRectangle::Load()
{
	if(m_min == m_max)
		{
			return;
		}

	std::vector<VertexModel> vertices = {
		VertexModel(Vector3(m_min, m_min, 0.0f), Vector2(0.0f, 0.0f)),
		VertexModel(Vector3(m_max, m_min, 0.0f), Vector2(1.0f, 0.0f)),
		VertexModel(Vector3(m_max, m_max, 0.0f), Vector2(1.0f, 1.0f)),
		VertexModel(Vector3(m_min, m_max, 0.0f), Vector2(0.0f, 1.0f)),
	};
	static std::vector<uint32_t> indices = {0, 1, 2, 2, 3, 0};

	Initialize(vertices, indices);
}

void ModelRectangle::Decode(const Metadata& metadata)
{
	metadata.GetChild("Min", m_min);
	metadata.GetChild("Max", m_max);
}

void ModelRectangle::Encode(Metadata& metadata) const
{
	metadata.SetChild<std::string>("Type", "ModelRectangle");
	metadata.SetChild("Min", m_min);
	metadata.SetChild("Max", m_max);
}
}
