#include "ModelCylinder.hpp"

#include "Maths/Maths.hpp"
#include "Models/VertexModel.hpp"
#include "Resources/Resources.hpp"

namespace acid
{
std::shared_ptr<ModelCylinder> ModelCylinder::Create(const Metadata& metadata)
{
	auto resource = Resources::Get()->Find(metadata);

	if(resource != nullptr)
		{
			return std::dynamic_pointer_cast<ModelCylinder>(resource);
		}

	auto result = std::make_shared<ModelCylinder>(0.0f, 0.0f);
	Resources::Get()->Add(metadata, std::dynamic_pointer_cast<Resource>(result));
	result->Decode(metadata);
	result->Load();
	return result;
}

std::shared_ptr<ModelCylinder> ModelCylinder::Create(const float& radiusBase, const float& radiusTop, const float& height, const uint32_t& slices, const uint32_t& stacks)
{
	auto temp = ModelCylinder(radiusBase, radiusTop, height, slices, stacks, false);
	Metadata metadata = Metadata();
	temp.Encode(metadata);
	return Create(metadata);
}

ModelCylinder::ModelCylinder(const float& radiusBase, const float& radiusTop, const float& height, const uint32_t& slices, const uint32_t& stacks, const bool& load) : m_radiusBase(radiusBase), m_radiusTop(radiusTop), m_height(height), m_slices(slices), m_stacks(stacks)
{
	if(load)
		{
			ModelCylinder::Load();
		}
}

void ModelCylinder::Load()
{
	if(m_radiusBase == 0.0f && m_radiusTop == 0.0f)
		{
			return;
		}

	std::vector<VertexModel> vertices = {};
	std::vector<uint32_t> indices = {};

	for(uint32_t i = 0; i < m_slices + 1; i++)
		{
			float iDivSlices = static_cast<float>(i) / static_cast<float>(m_slices);
			float alpha = (i == 0 || i == m_slices) ? 0.0f : iDivSlices * 2.0f * Maths::Pi;
			float xDir = std::cos(alpha);
			float zDir = std::sin(alpha);

			for(uint32_t j = 0; j < m_stacks + 1; j++)
				{
					float jDivStacks = static_cast<float>(j) / static_cast<float>(m_stacks);
					float radius = m_radiusBase * (1.0f - jDivStacks) + m_radiusTop * jDivStacks;

					Vector3 position = Vector3(xDir * radius, jDivStacks * m_height - (m_height / 2.0f), zDir * radius);
					Vector2 uvs = Vector2(1.0f - iDivSlices, 1.0f - jDivStacks);
					Vector3 normal = Vector3(xDir, 0.0f, zDir);
					vertices.emplace_back(position, uvs, normal);
				}
		}

	for(uint32_t i = 0; i < m_slices; i++)
		{
			for(uint32_t j = 0; j < m_stacks; j++)
				{
					uint32_t first = j + ((m_stacks + 1) * i);
					uint32_t second = j + ((m_stacks + 1) * (i + 1));

					indices.emplace_back(first + 1);
					indices.emplace_back(second + 1);
					indices.emplace_back(first);
					indices.emplace_back(second + 1);
					indices.emplace_back(second);
					indices.emplace_back(first);
				}
		}

	Initialize(vertices, indices);
}

void ModelCylinder::Decode(const Metadata& metadata)
{
	metadata.GetChild("Radius Base", m_radiusBase);
	metadata.GetChild("Radius Top", m_radiusTop);
	metadata.GetChild("Height", m_height);
	metadata.GetChild("Slices", m_slices);
	metadata.GetChild("Stacks", m_stacks);
}

void ModelCylinder::Encode(Metadata& metadata) const
{
	metadata.SetChild<std::string>("Type", "ModelCylinder");
	metadata.SetChild("Radius Base", m_radiusBase);
	metadata.SetChild("Radius Top", m_radiusTop);
	metadata.SetChild("Height", m_height);
	metadata.SetChild("Slices", m_slices);
	metadata.SetChild("Stacks", m_stacks);
}
}
