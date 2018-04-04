#include "MaterialDefault.hpp"

namespace Flounder
{
	MaterialDefault::MaterialDefault(const Colour &baseColor, Texture *diffuseTexture,
									 const float &metallic, const float &roughness, Texture *materialTexture, Texture *normalTexture,
									 const bool &castsShadows, const bool &ignoreLighting, const bool &ignoreFog) :
		m_baseColor(new Colour(baseColor)),
		m_diffuseTexture(diffuseTexture),
		m_metallic(metallic),
		m_roughness(roughness),
		m_materialTexture(materialTexture),
		m_normalTexture(normalTexture),
		m_castsShadows(castsShadows),
		m_ignoreLighting(ignoreLighting),
		m_ignoreFog(ignoreFog),
		m_pipeline(nullptr),
		m_descriptorSet(nullptr)
	{
		std::vector<Define> defines = {}; // {"ANIMATED"}, {"COLOUR_MAPPING"}, {"MATERIAL_MAPPING"}, {"NORMAL_MAPPING"}
		if (m_diffuseTexture != nullptr)
		{
			defines.push_back({"COLOUR_MAPPING"});
		}
		if (m_materialTexture != nullptr)
		{
			defines.push_back({"MATERIAL_MAPPING"});
		}
		/*if (m_normalTexture != nullptr)
		{
			defines.push_back({"NORMAL_MAPPING"});
		}*/
		m_pipeline = new Pipeline({1, 0}, PipelineCreate({"Resources/Shaders/Entities/Entity.vert", "Resources/Shaders/Entities/Entity.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MRT, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT), defines);
	}

	MaterialDefault::~MaterialDefault()
	{
		delete m_baseColor;
	//	delete m_diffuseTexture;
	//	delete m_materialTexture;
	//	delete m_normalTexture;

		delete m_pipeline;
	}

	void MaterialDefault::Update()
	{
	}

	void MaterialDefault::Load(LoadedValue *value)
	{
		m_baseColor->Set(value->GetChild("Base Colour")->GetString());
		TrySetDiffuseTexture(value->GetChild("Diffuse Texture")->GetString());

		m_metallic = value->GetChild("Metallic")->Get<float>();
		m_roughness = value->GetChild("Roughness")->Get<float>();
		TrySetMaterialTexture(value->GetChild("Material Texture")->GetString());
		TrySetNormalTexture(value->GetChild("Normal Texture")->GetString());

		m_castsShadows = (bool) value->GetChild("Casts Shadows")->Get<int>();
		m_ignoreLighting = (bool) value->GetChild("Ignore Lighting")->Get<int>();
		m_ignoreFog = (bool) value->GetChild("Ignore Fog")->Get<int>();
	}

	void MaterialDefault::Write(LoadedValue *destination)
	{
		destination->GetChild("Base Colour", true)->SetString(Colour::GetHex(*m_baseColor));
		destination->GetChild("Diffuse Texture", true)->SetString(m_diffuseTexture == nullptr ? "" : m_diffuseTexture->GetFilename());

		destination->GetChild("Metallic", true)->Set(m_metallic);
		destination->GetChild("Roughness", true)->Set(m_roughness);
		destination->GetChild("Material Texture", true)->SetString(m_materialTexture == nullptr ? "" : m_materialTexture->GetFilename());
		destination->GetChild("Normal Texture", true)->SetString(m_normalTexture == nullptr ? "" : m_normalTexture->GetFilename());

		destination->GetChild("Casts Shadows", true)->Set((int) m_castsShadows);
		destination->GetChild("Ignore Lighting", true)->Set((int) m_ignoreLighting);
		destination->GetChild("Ignore Fog", true)->Set((int) m_ignoreFog);
	}

	void MaterialDefault::CmdRender(const VkCommandBuffer &commandBuffer, Model *model, UniformBuffer *uniformScene, UniformBuffer *uniformObject, const unsigned int &instances)
	{
		// Binds this pipeline.
		m_pipeline->BindPipeline(commandBuffer);

		// Sets the descriptors.
		if (m_descriptorSet == nullptr)
		{
			m_descriptorSet = new DescriptorSet(*m_pipeline);
		}

		m_descriptorSet->UpdateMap({
			{"UboScene", uniformScene},
			{"UboObject", uniformObject},
			{"samplerDiffuse", m_diffuseTexture},
			{"samplerMaterial", m_materialTexture},
			{"samplerNormal", m_normalTexture}
		});

		// Draws the object.
		m_descriptorSet->BindDescriptor(commandBuffer);
		model->CmdRender(commandBuffer, instances);
	}
}
