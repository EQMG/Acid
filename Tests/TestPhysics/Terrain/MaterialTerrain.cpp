#include "MaterialTerrain.hpp"

#include <utility>
#include <Scenes/Entity.hpp>
#include <Models/VertexModel.hpp>

namespace test
{
	MaterialTerrain::MaterialTerrain(std::shared_ptr<Image2d> textureR, std::shared_ptr<Image2d> textureG) :
		m_textureR(std::move(textureR)),
		m_textureG(std::move(textureG))
	{
	}

	void MaterialTerrain::Start()
	{
		m_pipelineMaterial = PipelineMaterial::Create({1, 0}, PipelineGraphicsCreate({"Shaders/Terrains/Terrain.vert", "Shaders/Terrains/Terrain.frag"}, {VertexModel::GetVertexInput()}));
	}

	void MaterialTerrain::Update()
	{
	}

	void MaterialTerrain::Decode(const Metadata &metadata)
	{
		metadata.GetResource("Image2d R", m_textureR);
		metadata.GetResource("Image2d G", m_textureG);
	}

	void MaterialTerrain::Encode(Metadata &metadata) const
	{
		metadata.SetResource("Image2d R", m_textureR);
		metadata.SetResource("Image2d G", m_textureG);
	}

	void MaterialTerrain::PushUniforms(UniformHandler &uniformObject)
	{
		uniformObject.Push("transform", GetParent()->GetWorldMatrix());
	}

	void MaterialTerrain::PushDescriptors(DescriptorsHandler &descriptorSet)
	{
		descriptorSet.Push("samplerR", m_textureR);
		descriptorSet.Push("samplerG", m_textureG);
	}
}
