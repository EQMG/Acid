#include "ModelCylinder.hpp"

#include "Maths/Maths.hpp"
#include "Resources/Resources.hpp"
#include "Models/VertexDefault.hpp"

namespace acid
{
std::shared_ptr<ModelCylinder> ModelCylinder::Create(const Metadata &metadata)
{
	auto resource = Resources::Get()->Find(metadata);

	if (resource != nullptr)
	{
		return std::dynamic_pointer_cast<ModelCylinder>(resource);
	}

	auto result = std::make_shared<ModelCylinder>(0.0f, 0.0f);
	Resources::Get()->Add(metadata, std::dynamic_pointer_cast<Resource>(result));
	metadata >> *result;
	result->Load();
	return result;
}

std::shared_ptr<ModelCylinder> ModelCylinder::Create(const float &radiusBase, const float &radiusTop, const float &height, const uint32_t &slices, const uint32_t &stacks)
{
	auto temp = ModelCylinder(radiusBase, radiusTop, height, slices, stacks, false);
	Metadata metadata = Metadata();
	metadata << temp;
	return Create(metadata);
}

ModelCylinder::ModelCylinder(const float &radiusBase, const float &radiusTop, const float &height, const uint32_t &slices, const uint32_t &stacks, const bool &load) :
	m_radiusBase(radiusBase),
	m_radiusTop(radiusTop),
	m_height(height),
	m_slices(slices),
	m_stacks(stacks)
{
	if (load)
	{
		ModelCylinder::Load();
	}
}

void ModelCylinder::Load()
{
	if (m_radiusBase == 0.0f && m_radiusTop == 0.0f)
	{
		return;
	}

	std::vector<VertexDefault> vertices;
	std::vector<uint32_t> indices;
	// TODO: Reserve.

	for (uint32_t i = 0; i < m_slices + 1; i++)
	{
		float iDivSlices = static_cast<float>(i) / static_cast<float>(m_slices);
		float alpha = (i == 0 || i == m_slices) ? 0.0f : iDivSlices * 2.0f * Maths::Pi;
		float xDir = std::cos(alpha);
		float zDir = std::sin(alpha);

		for (uint32_t j = 0; j < m_stacks + 1; j++)
		{
			float jDivStacks = static_cast<float>(j) / static_cast<float>(m_stacks);
			float radius = m_radiusBase * (1.0f - jDivStacks) + m_radiusTop * jDivStacks;

			Vector3f position = Vector3f(xDir * radius, jDivStacks * m_height - (m_height / 2.0f), zDir * radius);
			Vector2f uvs = Vector2f(1.0f - iDivSlices, 1.0f - jDivStacks);
			Vector3f normal = Vector3f(xDir, 0.0f, zDir);
			vertices.emplace_back(VertexDefault(position, uvs, normal));
		}
	}

	for (uint32_t i = 0; i < m_slices; i++)
	{
		for (uint32_t j = 0; j < m_stacks; j++)
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

const Metadata &operator>>(const Metadata &metadata, ModelCylinder &model)
{
	metadata.GetChild("Radius Base", model.m_radiusBase);
	metadata.GetChild("Radius Top", model.m_radiusTop);
	metadata.GetChild("Height", model.m_height);
	metadata.GetChild("Slices", model.m_slices);
	metadata.GetChild("Stacks", model.m_stacks);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const ModelCylinder &model)
{
	metadata.SetChild<std::string>("Type", "ModelCylinder");
	metadata.SetChild("Radius Base", model.m_radiusBase);
	metadata.SetChild("Radius Top", model.m_radiusTop);
	metadata.SetChild("Height", model.m_height);
	metadata.SetChild("Slices", model.m_slices);
	metadata.SetChild("Stacks", model.m_stacks);
	return metadata;
}
}
