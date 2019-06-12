#include "ModelSphere.hpp"

#include "Maths/Maths.hpp"
#include "Resources/Resources.hpp"
#include "Models/VertexDefault.hpp"

namespace acid
{
std::shared_ptr<ModelSphere> ModelSphere::Create(const Metadata &metadata)
{
	auto resource{Resources::Get()->Find(metadata)};

	if (resource != nullptr)
	{
		return std::dynamic_pointer_cast<ModelSphere>(resource);
	}

	auto result{std::make_shared<ModelSphere>(0.0f)};
	Resources::Get()->Add(metadata, std::dynamic_pointer_cast<Resource>(result));
	metadata >> *result;
	result->Load();
	return result;
}

std::shared_ptr<ModelSphere> ModelSphere::Create(const float &radius, const uint32_t &latitudeBands, const uint32_t &longitudeBands)
{
	ModelSphere temp{radius, latitudeBands, longitudeBands, false};
	Metadata metadata;
	metadata << temp;
	return Create(metadata);
}

ModelSphere::ModelSphere(const float &radius, const uint32_t &latitudeBands, const uint32_t &longitudeBands, const bool &load) :
	m_radius(radius),
	m_latitudeBands(latitudeBands),
	m_longitudeBands(longitudeBands)
{
	if (load)
	{
		ModelSphere::Load();
	}
}

void ModelSphere::Load()
{
	if (m_radius == 0.0f)
	{
		return;
	}

	std::vector<VertexDefault> vertices;
	std::vector<uint32_t> indices;
	// TODO: Reserve.

	for (uint32_t i{}; i < m_longitudeBands + 1; i++)
	{
		auto iDivLong{static_cast<float>(i) / static_cast<float>(m_longitudeBands)};
		auto theta{(i == 0 || i == m_longitudeBands) ? 0.0f : iDivLong * 2.0f * Maths::Pi<float>};

		for (uint32_t j{}; j < m_latitudeBands + 1; j++)
		{
			auto jDivLat{static_cast<float>(j) / static_cast<float>(m_latitudeBands)};
			auto phi{jDivLat * 2.0f * Maths::Pi<float>};

			Vector3f normal{std::cos(phi) * std::sin(theta), std::cos(theta), std::sin(phi) * std::sin(theta)};
			auto position{m_radius * normal};
			Vector2f uvs{1.0f - jDivLat, 1.0f - iDivLong};
			vertices.emplace_back(VertexDefault{position, uvs, normal});
		}
	}

	for (uint32_t i{}; i < m_longitudeBands; i++)
	{
		for (uint32_t j{}; j < m_latitudeBands; j++)
		{
			auto first{j + ((m_latitudeBands + 1) * i)};
			auto second{j + ((m_latitudeBands + 1) * (i + 1))};

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

const Metadata &operator>>(const Metadata &metadata, ModelSphere &model)
{
	metadata.GetChild("latitudeBands", model.m_latitudeBands);
	metadata.GetChild("longitudeBands", model.m_longitudeBands);
	metadata.GetChild("radius", model.m_radius);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const ModelSphere &model)
{
	metadata.SetChild<std::string>("type", "ModelSphere");
	metadata.SetChild("latitudeBands", model.m_latitudeBands);
	metadata.SetChild("longitudeBands", model.m_longitudeBands);
	metadata.SetChild("radius", model.m_radius);
	return metadata;
}
}
