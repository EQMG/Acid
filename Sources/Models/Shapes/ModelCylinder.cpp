#include "ModelCylinder.hpp"

#include "Maths/Maths.hpp"
#include "Resources/Resources.hpp"
#include "Models/VertexModel.hpp"

namespace acid
{
	std::shared_ptr<ModelCylinder> ModelCylinder::Resource(const float &radiusBase, const float &radiusTop, const float &height, const uint32_t &slices, const uint32_t &stacks)
	{
		auto resource = Resources::Get()->Find(ToFilename(radiusBase, radiusTop, height, slices, stacks));

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<ModelCylinder>(resource);
		}

		auto result = std::make_shared<ModelCylinder>(radiusBase, radiusTop, height, slices, stacks);
		Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
		return result;
	}

	std::shared_ptr<ModelCylinder> ModelCylinder::Resource(const std::string &data)
	{
		if (data.empty())
		{
			return nullptr;
		}

		auto split = String::Split(data, "_");
		float radiusBase = String::From<float>(split[1]);
		float radiusTop = String::From<float>(split[2]);
		float height = String::From<float>(split[3]);
		uint32_t slices = String::From<uint32_t>(split[4]);
		uint32_t stacks = String::From<uint32_t>(split[5]);
		return Resource(radiusBase, radiusTop, height, slices, stacks);
	}

	ModelCylinder::ModelCylinder(const float &radiusBase, const float &radiusTop, const float &height, const uint32_t &slices, const uint32_t &stacks) :
		Model()
	{
		auto vertices = std::vector<VertexModel>();
		auto indices = std::vector<uint32_t>();

		for (uint32_t i = 0; i < slices + 1; i++)
		{
			float iDivSlices = static_cast<float>(i) / static_cast<float>(slices);
			float alpha = (i == 0 || i == slices) ? 0.0f : iDivSlices * 2.0f * PI;
			float xDir = std::cos(alpha);
			float zDir = std::sin(alpha);

			for (uint32_t j = 0; j < stacks + 1; j++)
			{
				float jDivStacks = static_cast<float>(j) / static_cast<float>(stacks);
				float radius = radiusBase * (1.0f - jDivStacks) + radiusTop * jDivStacks;

				Vector3 position = Vector3(xDir * radius, jDivStacks * height - (height / 2.0f), zDir * radius);
				Vector2 uvs = Vector2(1.0f - iDivSlices, 1.0f - jDivStacks);
				Vector3 normal = Vector3(xDir, 0.0f, zDir);
				Vector3 tangent = Vector3();
				vertices.emplace_back(VertexModel(position, uvs, normal, tangent));
			}
		}

		for (uint32_t i = 0; i < slices; i++)
		{
			for (uint32_t j = 0; j < stacks; j++)
			{
				uint32_t first = j + ((stacks + 1) * i);
				uint32_t second = j + ((stacks + 1) * (i + 1));

				indices.emplace_back(first);
				indices.emplace_back(second);
				indices.emplace_back(second + 1);

				indices.emplace_back(first);
				indices.emplace_back(second + 1);
				indices.emplace_back(first + 1);
			}
		}

		std::reverse(indices.begin(), indices.end());
		Model::Initialize(vertices, indices, ToFilename(radiusBase, radiusTop, height, slices, stacks));
	}

	std::string ModelCylinder::ToFilename(const float &radiusBase, const float &radiusTop, const float &height, const uint32_t &slices, const uint32_t &stacks)
	{
		std::stringstream result;
		result << "Cylinder_" << radiusBase << "_" << radiusTop << "_" << height << "_" << slices << "_" << stacks;
		return result.str();
	}
}
