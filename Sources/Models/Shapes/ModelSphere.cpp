#include "ModelSphere.hpp"

#include "Maths/Maths.hpp"
#include "Models/VertexModel.hpp"
#include "Resources/Resources.hpp"

namespace acid
{
std::shared_ptr<ModelSphere> ModelSphere::Create(const Metadata& metadata)
{
	auto resource = Resources::Get()->Find(metadata);

	if(resource != nullptr)
		{
			return std::dynamic_pointer_cast<ModelSphere>(resource);
		}

	auto result = std::make_shared<ModelSphere>(0.0f);
	Resources::Get()->Add(metadata, std::dynamic_pointer_cast<Resource>(result));
	result->Decode(metadata);
	result->Load();
	return result;
}

std::shared_ptr<ModelSphere> ModelSphere::Create(const float& radius, const uint32_t& latitudeBands, const uint32_t& longitudeBands)
{
	auto temp = ModelSphere(radius, latitudeBands, longitudeBands, false);
	Metadata metadata = Metadata();
	temp.Encode(metadata);
	return Create(metadata);
}

ModelSphere::ModelSphere(const float& radius, const uint32_t& latitudeBands, const uint32_t& longitudeBands, const bool& load) : m_radius(radius), m_latitudeBands(latitudeBands), m_longitudeBands(longitudeBands)
{
	if(load)
		{
			ModelSphere::Load();
		}
}

void ModelSphere::Load()
{
	if(m_radius == 0.0f)
		{
			return;
		}

	std::vector<VertexModel> vertices = {};
	std::vector<uint32_t> indices = {};

	for(uint32_t i = 0; i < m_longitudeBands + 1; i++)
		{
			float iDivLong = static_cast<float>(i) / static_cast<float>(m_longitudeBands);
			float theta = (i == 0 || i == m_longitudeBands) ? 0.0f : iDivLong * 2.0f * Maths::Pi;

			for(uint32_t j = 0; j < m_latitudeBands + 1; j++)
				{
					float jDivLat = static_cast<float>(j) / static_cast<float>(m_latitudeBands);
					float phi = jDivLat * 2.0f * Maths::Pi;

					Vector3 normal = Vector3(std::cos(phi) * std::sin(theta), std::cos(theta), std::sin(phi) * std::sin(theta));
					Vector3 position = m_radius * normal;
					Vector2 uvs = Vector2(1.0f - jDivLat, 1.0f - iDivLong);
					vertices.emplace_back(position, uvs, normal);
				}
		}

	for(uint32_t i = 0; i < m_longitudeBands; i++)
		{
			for(uint32_t j = 0; j < m_latitudeBands; j++)
				{
					uint32_t first = j + ((m_latitudeBands + 1) * i);
					uint32_t second = j + ((m_latitudeBands + 1) * (i + 1));

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

void ModelSphere::Decode(const Metadata& metadata)
{
	metadata.GetChild("Latitude Bands", m_latitudeBands);
	metadata.GetChild("Longitude Bands", m_longitudeBands);
	metadata.GetChild("Radius", m_radius);
}

void ModelSphere::Encode(Metadata& metadata) const
{
	metadata.SetChild<std::string>("Type", "ModelSphere");
	metadata.SetChild("Latitude Bands", m_latitudeBands);
	metadata.SetChild("Longitude Bands", m_longitudeBands);
	metadata.SetChild("Radius", m_radius);
}
}
