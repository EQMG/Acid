#include "ModelDisk.hpp"

#include "Maths/Maths.hpp"
#include "Resources/Resources.hpp"
#include "Models/VertexDefault.hpp"

namespace acid
{
std::shared_ptr<ModelDisk> ModelDisk::Create(const Metadata &metadata)
{
	auto resource{Resources::Get()->Find(metadata)};

	if (resource != nullptr)
	{
		return std::dynamic_pointer_cast<ModelDisk>(resource);
	}

	auto result{std::make_shared<ModelDisk>(0.0f, 0.0f)};
	Resources::Get()->Add(metadata, std::dynamic_pointer_cast<Resource>(result));
	metadata >> *result;
	result->Load();
	return result;
}

std::shared_ptr<ModelDisk> ModelDisk::Create(const float &innerRadius, const float &outerRadius, const uint32_t &slices, const uint32_t &loops)
{
	ModelDisk temp{innerRadius, outerRadius, slices, loops, false};
	Metadata metadata;
	metadata << temp;
	return Create(metadata);
}

ModelDisk::ModelDisk(const float &innerRadius, const float &outerRadius, const uint32_t &slices, const uint32_t &loops, const bool &load) :
	m_innerRadius(innerRadius),
	m_outerRadius(outerRadius),
	m_slices(slices),
	m_loops(loops)
{
	if (load)
	{
		Load();
	}
}

const Metadata &operator>>(const Metadata &metadata, ModelDisk &model)
{
	metadata.GetChild("innerRadius", model.m_innerRadius);
	metadata.GetChild("outerRadius", model.m_outerRadius);
	metadata.GetChild("slices", model.m_slices);
	metadata.GetChild("loops", model.m_loops);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const ModelDisk &model)
{
	metadata.SetChild<std::string>("type", "ModelDisk");
	metadata.SetChild("innerRadius", model.m_innerRadius);
	metadata.SetChild("outerRadius", model.m_outerRadius);
	metadata.SetChild("slices", model.m_slices);
	metadata.SetChild("loops", model.m_loops);
	return metadata;
}

void ModelDisk::Load()
{
	if (m_innerRadius == 0.0f && m_outerRadius == 0.0f)
	{
		return;
	}

	std::vector<VertexDefault> vertices;
	std::vector<uint32_t> indices;
	vertices.reserve(m_slices * (m_loops + 1));
	indices.reserve(m_slices * m_loops * 6);

	for (uint32_t i{}; i < m_slices; i++)
	{
		auto iDivSlices{static_cast<float>(i) / static_cast<float>(m_slices)};
		auto alpha{iDivSlices * 2.0f * Maths::Pi<float>};
		auto xDir{std::cos(alpha)};
		auto yDir{std::sin(alpha)};

		for (uint32_t j{}; j < m_loops + 1; j++)
		{
			auto jDivLoops{static_cast<float>(j) / static_cast<float>(m_loops)};
			auto radius{m_innerRadius + jDivLoops * (m_outerRadius - m_innerRadius)};

			Vector3f position{radius * xDir, 0.0f, radius * yDir};
			Vector2f uvs{1.0f - iDivSlices, 1.0f - jDivLoops};
			Vector3f normal{0.0f, 1.0f, 0.0f};
			vertices.emplace_back(VertexDefault{position, uvs, normal});
		}
	}

	for (uint32_t i{}; i < m_slices; i++)
	{
		for (uint32_t j{}; j < m_loops; j++)
		{
			auto first{i * (m_loops + 1) + j};
			auto second{(first + m_loops + 1) % (m_slices * (m_loops + 1))};

			indices.emplace_back(second + 1);
			indices.emplace_back(first + 1);
			indices.emplace_back(first);
			indices.emplace_back(second);
			indices.emplace_back(second + 1);
			indices.emplace_back(first);
		}
	}

	Initialize(vertices, indices);
}
}
