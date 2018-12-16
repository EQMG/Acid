#include "ModelDisk.hpp"

#include "Maths/Maths.hpp"
#include "Resources/Resources.hpp"
#include "Models/VertexModel.hpp"

namespace acid
{
	std::shared_ptr<ModelDisk> ModelDisk::Resource(const float &innerRadius, const float &outerRadius, const uint32_t &slices, const uint32_t &loops)
	{
		auto resource = Resources::Get()->Find(ToFilename(innerRadius, outerRadius, slices, loops));

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<ModelDisk>(resource);
		}

		auto result = std::make_shared<ModelDisk>(innerRadius, outerRadius, slices, loops);
		Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
		return result;
	}

	std::shared_ptr<ModelDisk> ModelDisk::Resource(const std::string &data)
	{
		if (data.empty())
		{
			return nullptr;
		}

		auto split = String::Split(data, "_");
		float innerRadius = String::From<float>(split[1]);
		float outerRadius = String::From<float>(split[2]);
		uint32_t slices = String::From<uint32_t>(split[3]);
		uint32_t loops = String::From<uint32_t>(split[4]);
		return Resource(innerRadius, outerRadius, slices, loops);
	}

	ModelDisk::ModelDisk(const float &innerRadius, const float &outerRadius, const uint32_t &slices, const uint32_t &loops) :
		Model()
	{
		auto vertices = std::vector<VertexModel>();
		auto indices = std::vector<uint32_t>();

		for (uint32_t i = 0; i < slices; i++)
		{
			float iDivSlices = static_cast<float>(i) / static_cast<float>(slices);
			float alpha = iDivSlices * 2.0f * PI;
			float xDir = cos(alpha), yDir = sin(alpha);

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
		Model::Initialize(vertices, indices, ToFilename(innerRadius, outerRadius, slices, loops));
	}

	std::string ModelDisk::ToFilename(const float &innerRadius, const float &outerRadius, const uint32_t &slices, const uint32_t &loops)
	{
		std::stringstream result;
		result << "Disk_" << innerRadius << "_" << outerRadius << "_" << slices << "_" << loops;
		return result.str();
	}
}
