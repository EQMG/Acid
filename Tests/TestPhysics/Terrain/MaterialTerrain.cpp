#include "MaterialTerrain.hpp"

#include <utility>
#include <Scenes/Entity.hpp>
#include <Models/VertexDefault.hpp>
#include <Maths/Transform.hpp>

namespace test
{
MaterialTerrain::MaterialTerrain(std::shared_ptr<Image2d> imageR, std::shared_ptr<Image2d> imageG) :
	m_imageR{std::move(imageR)},
	m_imageG{std::move(imageG)}
{
}

void MaterialTerrain::Start()
{
	m_pipelineMaterial = PipelineMaterial::Create({1, 0}, {{"Shaders/Terrains/Terrain.vert", "Shaders/Terrains/Terrain.frag"}, {VertexDefault::GetVertexInput()}});
}

void MaterialTerrain::Update()
{
}

void MaterialTerrain::PushUniforms(UniformHandler &uniformObject)
{
	if (auto transform{GetEntity()->GetComponent<Transform>()}; transform != nullptr)
	{
		uniformObject.Push("transform", transform->GetWorldMatrix());
	}
}

void MaterialTerrain::PushDescriptors(DescriptorsHandler &descriptorSet)
{
	descriptorSet.Push("samplerR", m_imageR);
	descriptorSet.Push("samplerG", m_imageG);
}

const Metadata &operator>>(const Metadata &metadata, MaterialTerrain &material)
{
	metadata.GetChild("imageR", material.m_imageR);
	metadata.GetChild("imageG", material.m_imageG);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const MaterialTerrain &material)
{
	metadata.SetChild("imageR", material.m_imageR);
	metadata.SetChild("imageG", material.m_imageG);
	return metadata;
}
}
