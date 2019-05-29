#include "ModelRectangle.hpp"

#include "Resources/Resources.hpp"
#include "Models/VertexDefault.hpp"

namespace acid
{
std::shared_ptr<ModelRectangle> ModelRectangle::Create(const Metadata &metadata)
{
	auto resource = Resources::Get()->Find(metadata);

	if (resource != nullptr)
	{
		return std::dynamic_pointer_cast<ModelRectangle>(resource);
	}

	auto result = std::make_shared<ModelRectangle>(0.0f, 0.0f);
	Resources::Get()->Add(metadata, std::dynamic_pointer_cast<Resource>(result));
	metadata >> *result;
	result->Load();
	return result;
}

std::shared_ptr<ModelRectangle> ModelRectangle::Create(const float &min, const float &max)
{
	auto temp = ModelRectangle(min, max, false);
	Metadata metadata = Metadata();
	metadata << temp;
	return Create(metadata);
}

ModelRectangle::ModelRectangle(const float &min, const float &max, const bool &load) :
	m_min(min),
	m_max(max)
{
	if (load)
	{
		ModelRectangle::Load();
	}
}

void ModelRectangle::Load()
{
	if (m_min == m_max)
	{
		return;
	}

	std::vector<VertexDefault> vertices = { 
		VertexDefault(Vector3f(m_min, m_min, 0.0f), Vector2f(0.0f, 0.0f), Vector3f()),
		VertexDefault(Vector3f(m_max, m_min, 0.0f), Vector2f(1.0f, 0.0f), Vector3f()), 
		VertexDefault(Vector3f(m_max, m_max, 0.0f), Vector2f(1.0f, 1.0f), Vector3f()),
		VertexDefault(Vector3f(m_min, m_max, 0.0f), Vector2f(0.0f, 1.0f), Vector3f())
	};
	static std::vector<uint32_t> indices = { 
		0, 1, 2, 
		2, 3, 0 
	};

	Initialize(vertices, indices);
}

const Metadata &operator>>(const Metadata &metadata, ModelRectangle &model)
{
	metadata.GetChild("Min", model.m_min);
	metadata.GetChild("Max", model.m_max);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const ModelRectangle &model)
{
	metadata.SetChild<std::string>("Type", "ModelRectangle");
	metadata.SetChild("Min", model.m_min);
	metadata.SetChild("Max", model.m_max);
	return metadata;
}
}
