#include "ModelDisk.hpp"

#include "Maths/Maths.hpp"
#include "Resources/Resources.hpp"
#include "Models/VertexModel.hpp"

namespace acid
{
	std::shared_ptr<ModelDisk> ModelDisk::Create(const float &innerRadius, const float &outerRadius, const uint32_t &slices, const uint32_t &loops)
	{
		std::shared_ptr<Resource> resource = nullptr; // FIXME: Resources::Get()->Find(ToName(innerRadius, outerRadius, slices, loops));

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<ModelDisk>(resource);
		}

		auto result = std::make_shared<ModelDisk>(innerRadius, outerRadius, slices, loops);
		// Resources::Get()->Add(std::dynamic_pointer_cast<Resource>(result));
		return result;
	}

	ModelDisk::ModelDisk(const float &innerRadius, const float &outerRadius, const uint32_t &slices, const uint32_t &loops) :
		Model(),
		m_innerRadius(innerRadius),
		m_outerRadius(outerRadius),
		m_slices(slices),
		m_loops(loops)
	{
		std::vector<VertexModel> vertices = {};
		std::vector<uint32_t> indices = {};

		for (uint32_t i = 0; i < slices; i++)
		{
			float iDivSlices = static_cast<float>(i) / static_cast<float>(slices);
			float alpha = iDivSlices * 2.0f * Maths::Pi;
			float xDir = std::cos(alpha), yDir = std::sin(alpha);

			for (uint32_t j = 0; j < loops + 1; j++)
			{
				float jDivLoops = static_cast<float>(j) / static_cast<float>(loops);
				float radius = innerRadius + jDivLoops * (outerRadius - innerRadius);

				Vector3 position = Vector3(radius * xDir, 0.0f, radius * yDir);
				Vector2 uvs = Vector2(1.0f - iDivSlices, 1.0f - jDivLoops);
				Vector3 normal = Vector3(0.0f, 1.0f, 0.0f);
				Vector3 tangent = Vector3();
				vertices.emplace_back(VertexModel(position, uvs, normal, tangent));
			}
		}

		for (uint32_t i = 0; i < slices; i++)
		{
			for (uint32_t j = 0; j < loops; j++)
			{
				uint32_t first = i * (loops + 1) + j;
				uint32_t second = (first + loops + 1) % (slices * (loops + 1));

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

	/*void ModelDisk::Decode(const Metadata &metadata)
	{
		m_innerRadius = metadata.GetChild<float>("Inner Radius");
		m_outerRadius = metadata.GetChild<float>("Outer Radius");
		m_slices = metadata.GetChild<uint32_t>("Slices");
		m_loops = metadata.GetChild<uint32_t>("Loops");
	}*/

	void ModelDisk::Encode(Metadata &metadata) const
	{
		metadata.SetChild<std::string>("Type", "ModelDisk");
		metadata.SetChild<float>("Inner Radius", m_innerRadius);
		metadata.SetChild<float>("Outer Radius", m_outerRadius);
		metadata.SetChild<uint32_t>("Slices", m_slices);
		metadata.SetChild<uint32_t>("Loops", m_loops);
	}
}
