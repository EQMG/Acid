#include "Material.hpp"

namespace Flounder
{
	Material::Material(const Colour &baseColor, Texture *diffuse, const float &metallic, const float &roughness, const bool &castsShadows, const bool &ignoreLighting, const bool &ignoreFog, Texture *material, Texture *normal) :
		Component(),
		m_baseColor(new Colour(baseColor)),
		m_textureDiffuse(diffuse),
		m_metallic(metallic),
		m_roughness(roughness),
		m_castsShadows(castsShadows),
		m_ignoreLighting(ignoreLighting),
		m_ignoreFog(ignoreFog),
		m_textureMaterial(material), // R: Metallic, G: Roughness, B: Emmisive
		m_textureNormal(normal)
	{
	}

	Material::~Material()
	{
		delete m_baseColor;
	}

	void Material::Update()
	{
		Component::Update();
	}

	void Material::Load(LoadedValue *value)
	{
		TrySetTextureDiffuse(value->GetChild("Diffuse Texture")->GetRaw());
		m_baseColor->Set(value->GetChild("Base Colour")->GetRaw());
		m_metallic = value->GetChild("Metallic")->Get<float>();
		m_roughness = value->GetChild("Roughness")->Get<float>();
		m_castsShadows = value->GetChild("Cast Shadows")->Get<bool>();
		m_ignoreLighting = value->GetChild("Ignore Lighting")->Get<bool>();
		m_ignoreFog = value->GetChild("Ignore Fog")->Get<bool>();
		TrySetTextureMaterial(value->GetChild("Material Texture")->GetRaw());
		TrySetTextureNormal(value->GetChild("Normal Texture")->GetRaw());
	}

	void Material::Write(LoadedValue *value)
	{
		value->GetChild("Diffuse Texture", true)->SetRaw(m_textureDiffuse == nullptr ? "" : m_textureDiffuse->GetFilename());
		value->GetChild("Base Colour", true)->SetRaw(Colour::GetHex(*m_baseColor));
		value->GetChild("Metallic", true)->Set(m_metallic);
		value->GetChild("Roughness", true)->Set(m_roughness);
		value->GetChild("Ignore Lighting", true)->Set(m_ignoreLighting);
		value->GetChild("Ignore Fog", true)->Set(m_ignoreFog);
		value->GetChild("Material Texture", true)->SetRaw(m_textureMaterial == nullptr ? "" : m_textureMaterial->GetFilename());
		value->GetChild("Normal Texture", true)->SetRaw(m_textureNormal == nullptr ? "" : m_textureNormal->GetFilename());
	}

	void Material::TrySetTextureDiffuse(const std::string &filename)
	{
		if (!filename.empty())
		{
			m_textureDiffuse = Texture::Resource(filename);
		}
	}

	void Material::TrySetTextureMaterial(const std::string &filename)
	{
		if (!filename.empty())
		{
			m_textureMaterial = Texture::Resource(filename);
		}
	}

	void Material::TrySetTextureNormal(const std::string &filename)
	{
		if (!filename.empty())
		{
			m_textureNormal = Texture::Resource(filename);
		}
	}
}
