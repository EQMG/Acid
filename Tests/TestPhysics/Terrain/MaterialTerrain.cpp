#include "MaterialTerrain.hpp"

#include <Objects/GameObject.hpp>
#include <Models/VertexModel.hpp>

namespace test
{
	MaterialTerrain::MaterialTerrain(const std::shared_ptr<Texture> &textureR, const std::shared_ptr<Texture> &textureG) :
		IMaterial(),
		m_material(PipelineMaterial::Resource({1, 0}, PipelineCreate({"Shaders/Terrains/Terrain.vert", "Shaders/Terrains/Terrain.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MODE_MRT, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, {}))),
		m_textureR(textureR),
		m_textureG(textureG)
	{
	}

	MaterialTerrain::~MaterialTerrain()
	{
	}

	void MaterialTerrain::Start()
	{
	}

	void MaterialTerrain::Update()
	{
	}

	void MaterialTerrain::Decode(const Node &node)
	{
		TrySetTextureR(node.GetChild<std::string>("Texture R"));
		TrySetTextureR(node.GetChild<std::string>("Texture G"));
	}

	void MaterialTerrain::Encode(Node &node) const
	{
		node.SetChild<std::string>("Texture R", m_textureR == nullptr ? "" : m_textureR->GetFilename());
		node.SetChild<std::string>("Texture G", m_textureG == nullptr ? "" : m_textureG->GetFilename());
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
