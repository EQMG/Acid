#include "ModelRectangle.hpp"

#include "Resources/Resources.hpp"
#include "Models/VertexDefault.hpp"

namespace acid
{
std::shared_ptr<ModelRectangle> ModelRectangle::Create(const Metadata &metadata)
{
	auto resource{Resources::Get()->Find(metadata)};

	if (resource != nullptr)
	{
		return std::dynamic_pointer_cast<ModelRectangle>(resource);
	}

	auto result{std::make_shared<ModelRectangle>(0.0f, 0.0f)};
	Resources::Get()->Add(metadata, std::dynamic_pointer_cast<Resource>(result));
	metadata >> *result;
	result->Load();
	return result;
}

std::shared_ptr<ModelRectangle> ModelRectangle::Create(const float &min, const float &max)
{
	ModelRectangle temp{min, max, false};
	Metadata metadata;
	metadata << temp;
	return Create(metadata);
}

ModelRectangle::ModelRectangle(const float &min, const float &max, const bool &load) :
	m_min(min),
	m_max(max)
{
	if (load)
	{
		Load();
	}
}

const Metadata &operator>>(const Metadata &metadata, ModelRectangle &model)
{
	metadata.GetChild("min", model.m_min);
	metadata.GetChild("max", model.m_max);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const ModelRectangle &model)
{
	metadata.SetChild<std::string>("type", "ModelRectangle");
	metadata.SetChild("min", model.m_min);
	metadata.SetChild("max", model.m_max);
	return metadata;
}

void ModelRectangle::Load()
{
	if (m_min == m_max)
	{
		return;
	}

	std::vector<VertexDefault> vertices{
		{{m_min, m_min, 0.0f}, {0.0f, 0.0f}, {}},
		{{m_max, m_min, 0.0f}, {1.0f, 0.0f}, {}},
		{{m_max, m_max, 0.0f}, {1.0f, 1.0f}, {}},
		{{m_min, m_max, 0.0f}, {0.0f, 1.0f}, {}}
	};
	static std::vector<uint32_t> indices{
		0, 1, 2,
		2, 3, 0
	};

	Initialize(vertices, indices);
}
}
