#include "ModelCylinder.hpp"

#include "Maths/Maths.hpp"
#include "Resources/Resources.hpp"
#include "Models/VertexModel.hpp"

namespace acid
{
	std::shared_ptr<ModelCylinder> ModelCylinder::Create(const float &radiusBase, const float &radiusTop, const float &height, const uint32_t &slices, const uint32_t &stacks)
	{
		std::shared_ptr<Resource> resource = nullptr; // FIXME: Resources::Get()->Find(ToName(radiusBase, radiusTop, height, slices, stacks));

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<ModelCylinder>(resource);
		}

		auto result = std::make_shared<ModelCylinder>(radiusBase, radiusTop, height, slices, stacks);
		// Resources::Get()->Add(std::dynamic_pointer_cast<Resource>(result));
		return result;
	}

	ModelCylinder::ModelCylinder(const float &radiusBase, const float &radiusTop, const float &height, const uint32_t &slices, const uint32_t &stacks) :
		Model(),
		m_radiusBase(radiusBase),
		m_radiusTop(radiusTop),
		m_height(height),
		m_slices(slices),
		m_stacks(stacks)
	{
		std::vector<VertexModel> vertices = {};
		std::vector<uint32_t> indices = {};

		for (uint32_t i = 0; i < slices + 1; i++)
		{
			float iDivSlices = static_cast<float>(i) / static_cast<float>(slices);
			float alpha = (i == 0 || i == slices) ? 0.0f : iDivSlices * 2.0f * Maths::Pi;
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
		Model::Initialize(vertices, indices);
	}

	/*void ModelCylinder::Decode(const Metadata &metadata)
	{
		m_radiusBase = metadata.GetChild<float>("Radius Base");
		m_radiusTop = metadata.GetChild<float>("Radius Top");
		m_height = metadata.GetChild<float>("Height");
		m_slices = metadata.GetChild<uint32_t>("Slices");
		m_stacks = metadata.GetChild<uint32_t>("Stacks");
	}*/

	void ModelCylinder::Encode(Metadata &metadata) const
	{
		metadata.SetChild<std::string>("Type", "ModelCylinder");
		metadata.SetChild<float>("Radius Base", m_radiusBase);
		metadata.SetChild<float>("Radius Top", m_radiusTop);
		metadata.SetChild<float>("Height", m_height);
		metadata.SetChild<uint32_t>("Slices", m_slices);
		metadata.SetChild<uint32_t>("Stacks", m_stacks);
	}
}
