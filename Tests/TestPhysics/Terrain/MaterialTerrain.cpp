#include "MaterialTerrain.hpp"

#include <Scenes/Entity.hpp>
#include <Models/VertexModel.hpp>

namespace test
{
	MaterialTerrain::MaterialTerrain(const std::shared_ptr<Texture> &textureR, const std::shared_ptr<Texture> &textureG) :
		m_textureR(textureR),
		m_textureG(textureG)
	{
	}

	void MaterialTerrain::Start()
	{
		m_pipelineMaterial = PipelineMaterial::Create({1, 0}, PipelineGraphicsCreate({"Shaders/Terrains/Terrain.vert", "Shaders/Terrains/Terrain.frag"}, {VertexModel::GetVertexInput()},
			PipelineGraphics::Mode::Mrt, PipelineGraphics::Depth::ReadWrite, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, {}));
	}

	void MaterialTerrain::Update()
	{
	}

	void MaterialTerrain::Decode(const Metadata &metadata)
	{
		m_textureR = Texture::Create(metadata.GetChild<std::string>("Texture R"));
		m_textureG = Texture::Create(metadata.GetChild<std::string>("Texture G"));
	}

	void MaterialTerrain::Encode(Metadata &metadata) const
	{
		metadata.SetChild<std::shared_ptr<Texture>>("Texture R", m_textureR);
		metadata.SetChild<std::shared_ptr<Texture>>("Texture G", m_textureG);
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
