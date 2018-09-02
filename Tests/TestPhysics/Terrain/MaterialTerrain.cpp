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

	void MaterialTerrain::Load(LoadedValue &value)
	{
		TrySetTextureR(value.GetChild("Texture R")->GetString());
		TrySetTextureR(value.GetChild("Texture G")->GetString());
	}

	void MaterialTerrain::Write(LoadedValue &destination)
	{
		destination.GetChild("Texture R", true)->SetString(m_textureR == nullptr ? "" : m_textureR->GetFilename());
		destination.GetChild("Texture G", true)->SetString(m_textureG == nullptr ? "" : m_textureG->GetFilename());
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
