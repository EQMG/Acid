#include "MaterialDefault.hpp"

namespace Flounder
{
	MaterialDefault::MaterialDefault(const Colour &baseColor, Texture *diffuseTexture,
					   const float &metallic, const float &roughness, const bool &ignoreLighting, const bool &ignoreFog, Texture *materialTexture,
					   Texture *normalTexture) :
		m_baseColor(new Colour(baseColor)),
		m_metallic(metallic),
		m_roughness(roughness),
		m_ignoreLighting(ignoreLighting),
		m_ignoreFog(ignoreFog),
		m_materialTexture(materialTexture),
		m_normalTexture(normalTexture)
	{
	}

	MaterialDefault::~MaterialDefault()
	{
		delete m_baseColor;
	//	delete m_diffuseTexture;

	//	delete m_materialTexture;

	//	delete m_normalTexture;
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
		m_ignoreLighting = value->GetChild("Ignore Lighting")->Get<bool>();
		m_ignoreFog = value->GetChild("Ignore Fog")->Get<bool>();
		TrySetMaterialTexture(value->GetChild("Material Texture")->GetString());

		TrySetNormalTexture(value->GetChild("Normal Texture")->GetString());
	}

	void MaterialDefault::Write(LoadedValue *destination)
	{
		destination->GetChild("Diffuse Texture", true)->SetString(m_diffuseTexture == nullptr ? "" : m_diffuseTexture->GetFilename());
		destination->GetChild("Colour", true)->SetString(Colour::GetHex(*m_baseColor));

		destination->GetChild("Metallic", true)->Set(m_metallic);
		destination->GetChild("Roughness", true)->Set(m_roughness);
		destination->GetChild("Ignore Lighting", true)->Set(m_ignoreLighting);
		destination->GetChild("Ignore Fog", true)->Set(m_ignoreFog);
		destination->GetChild("Material Texture", true)->SetString(m_materialTexture == nullptr ? "" : m_materialTexture->GetFilename());

		destination->GetChild("Normal Texture", true)->SetString(m_normalTexture == nullptr ? "" : m_normalTexture->GetFilename());
	}
}
