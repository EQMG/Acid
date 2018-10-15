#include "MaterialTerrain.hpp"

#include <Objects/GameObject.hpp>
#include <Models/VertexModel.hpp>

namespace test
{
	MaterialTerrain::MaterialTerrain(const std::shared_ptr<Texture> &textureR, const std::shared_ptr<Texture> &textureG) :
		m_textureR(textureR),
		m_textureG(textureG),
		m_material(nullptr)
	{
	}

	void MaterialTerrain::Start()
	{
		m_material = PipelineMaterial::Resource({1, 0}, PipelineCreate({"Shaders/Terrains/Terrain.vert", "Shaders/Terrains/Terrain.frag"}, {VertexModel::GetVertexInput()},
			PIPELINE_MODE_MRT, PIPELINE_DEPTH_READ_WRITE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, {}));
	}

	void MaterialTerrain::Update()
	{
	}

	void MaterialTerrain::Decode(const Metadata &metadata)
	{
		TrySetTextureR(metadata.GetChild<std::string>("Texture R"));
		TrySetTextureR(metadata.GetChild<std::string>("Texture G"));
	}

	void MaterialTerrain::Encode(Metadata &metadata) const
	{
		metadata.SetChild<std::string>("Texture R", m_textureR == nullptr ? "" : m_textureR->GetFilename());
		metadata.SetChild<std::string>("Texture G", m_textureG == nullptr ? "" : m_textureG->GetFilename());
	}

	void MaterialTerrain::PushUniforms(UniformHandler &uniformObject)
	{
		uniformObject.Push("transform", GetGameObject()->GetTransform().GetWorldMatrix());
	}

	void MaterialTerrain::PushDescriptors(DescriptorsHandler &descriptorSet)
	{
		descriptorSet.Push("samplerR", m_textureR);
		descriptorSet.Push("samplerG", m_textureG);
	}
}
