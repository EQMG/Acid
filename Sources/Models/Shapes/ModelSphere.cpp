#include "ModelSphere.hpp"

#include "Maths/Maths.hpp"
#include "Resources/Resources.hpp"
#include "Models/VertexModel.hpp"

namespace acid
{
	std::shared_ptr<ModelSphere> ModelSphere::Create(const uint32_t &latitudeBands, const uint32_t &longitudeBands, const float &radius)
	{
		std::shared_ptr<Resource> resource = nullptr; // FIXME: Resources::Get()->Find(ToName(latitudeBands, longitudeBands, radius));

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<ModelSphere>(resource);
		}

		auto result = std::make_shared<ModelSphere>(latitudeBands, longitudeBands, radius);
		// Resources::Get()->Add(std::dynamic_pointer_cast<Resource>(result));
		return result;
	}

	ModelSphere::ModelSphere(const uint32_t &latitudeBands, const uint32_t &longitudeBands, const float &radius) :
		Model(),
		m_latitudeBands(latitudeBands),
		m_longitudeBands(longitudeBands),
		m_radius(radius)
	{
		std::vector<VertexModel> vertices = {};
		std::vector<uint32_t> indices = {};

		for (uint32_t i = 0; i < longitudeBands + 1; i++)
		{
			float iDivLong = static_cast<float>(i) / static_cast<float>(longitudeBands);
			float theta = (i == 0 || i == longitudeBands) ? 0.0f : iDivLong * 2.0f * Maths::Pi;

			for (uint32_t j = 0; j < latitudeBands + 1; j++)
			{
				float jDivLat = static_cast<float>(j) / static_cast<float>(latitudeBands);
				float phi = jDivLat * 2.0f * Maths::Pi;

				Vector3 normal = Vector3(std::cos(phi) * std::sin(theta), std::cos(theta), std::sin(phi) * std::sin(theta));
				Vector3 position = radius * normal;
				Vector2 uvs = Vector2(1.0f - jDivLat, 1.0f - iDivLong);
				Vector3 tangent = Vector3();
				vertices.emplace_back(VertexModel(position, uvs, normal, tangent));
			}
		}

		for (uint32_t i = 0; i < longitudeBands; i++)
		{
			for (uint32_t j = 0; j < latitudeBands; j++)
			{
				uint32_t first = j + ((latitudeBands + 1) * i);
				uint32_t second = j + ((latitudeBands + 1) * (i + 1));

				indices.emplace_back(first);
				indices.emplace_back(second + 1);
				indices.emplace_back(second);

				indices.emplace_back(first);
				indices.emplace_back(first + 1);
				indices.emplace_back(second + 1);
			}
		}

		std::reverse(indices.begin(), indices.end());
		Model::Initialize(vertices, indices);
	}

	/*void ModelSphere::Decode(const Metadata &metadata)
	{
		m_latitudeBands = metadata.GetChild<uint32_t>("Latitude Bands");
		m_longitudeBands = metadata.GetChild<uint32_t>("Longitude Bands");
		m_radius = metadata.GetChild<float>("Radius");
	}*/

	void ModelSphere::Encode(Metadata &metadata) const
	{
		metadata.SetChild<std::string>("Type", "ModelSphere");
		metadata.SetChild<uint32_t>("Latitude Bands", m_latitudeBands);
		metadata.SetChild<uint32_t>("Longitude Bands", m_longitudeBands);
		metadata.SetChild<float>("Radius", m_radius);
	}
}
