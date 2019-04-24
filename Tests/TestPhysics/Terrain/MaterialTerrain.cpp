#include "MaterialTerrain.hpp"

#include <utility>
#include <Scenes/Entity.hpp>
#include <Models/VertexDefault.hpp>

namespace test
{
MaterialTerrain::MaterialTerrain(std::shared_ptr<Image2d> imageR, std::shared_ptr<Image2d> imageG) :
	m_imageR(std::move(imageR)),
	m_imageG(std::move(imageG))
{
}

void MaterialTerrain::Start()
{
	m_pipelineMaterial = PipelineMaterial::Create({ 1, 0 },
		PipelineGraphicsCreate({ "Shaders/Terrains/Terrain.vert", "Shaders/Terrains/Terrain.frag" }, { VertexDefault::GetVertexInput() }));
}

void MaterialTerrain::Update()
{
}

void MaterialTerrain::Decode(const Metadata &metadata)
{
	metadata.GetResource("Image R", m_imageR);
	metadata.GetResource("Image G", m_imageG);
}

void MaterialTerrain::Encode(Metadata &metadata) const
{
	metadata.SetResource("Image R", m_imageR);
	metadata.SetResource("Image G", m_imageG);
}

void MaterialTerrain::PushUniforms(UniformHandler &uniformObject)
{
	uniformObject.Push("transform", GetParent()->GetWorldMatrix());
}

void MaterialTerrain::PushDescriptors(DescriptorsHandler &descriptorSet)
{
	descriptorSet.Push("samplerR", m_imageR);
	descriptorSet.Push("samplerG", m_imageG);
}
}
