#pragma once

#include "../Objects/Component.hpp"
#include "../Maths/Colour.hpp"
#include "../Textures/Texture.hpp"

namespace Flounder
{
	/*class F_EXPORT MaterialDiffuse
	{
		Colour *m_baseColor;
		Texture *m_textureDiffuse;

		MaterialDiffuse::MaterialDiffuse(const Colour &baseColor, Texture *diffuse) :
			Component(),
			m_baseColor(new Colour(baseColor)),
			m_textureDiffuse(diffuse)
		{
		}
	};

	class F_EXPORT MaterialSurface
	{
		float m_metallic;
		float m_roughness;
		bool m_castsShadows;
		bool m_ignoreLighting;
		bool m_ignoreFog;
		Texture *m_textureMaterial; // R: Metallic, G: Roughness, B: Emmisive

		MaterialSurface::MaterialSurface(const float &metallic, const float &roughness, const bool &castsShadows, const bool &ignoreLighting, const bool &ignoreFog, Texture *material) :
			m_metallic(metallic),
			m_roughness(roughness),
			m_castsShadows(castsShadows),
			m_ignoreLighting(ignoreLighting),
			m_ignoreFog(ignoreFog),
			m_textureMaterial(material)
		{
		}
	};

	class F_EXPORT MaterialNormal
	{
		Texture *m_textureNormal;

		MaterialNormal::MaterialNormal(Texture *normal) :
			m_textureNormal(normal)
		{
		}
	};*/

	/// <summary>
	/// Class that represents a material.
	/// </summary>
	class F_EXPORT Material :
		public Component
	{
	private:
		Colour *m_baseColor;
		Texture *m_textureDiffuse;

		float m_metallic;
		float m_roughness;
		bool m_castsShadows;
		bool m_ignoreLighting;
		bool m_ignoreFog;

		Texture *m_textureMaterial;
		Texture *m_textureNormal;
	public:
		Material(const Colour &baseColor = Colour::WHITE, Texture *diffuse = nullptr, const float &metallic = 0.0f, const float &roughness = 0.0f, const bool &castsShadows = true, const bool &ignoreLighting = false, const bool &ignoreFog = false, Texture *material = nullptr, Texture *normal = nullptr);

		~Material();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *value) override;

		std::string GetName() const override { return "Material"; };

		Colour *GetBaseColor() const { return m_baseColor; }

		void SetBaseColor(const Colour &baseColor) { m_baseColor->Set(baseColor); }

		Texture *GetTextureDiffuse() const { return m_textureDiffuse; }

		void SetTextureDiffuse(Texture *diffuse) { m_textureDiffuse = diffuse; }

		void TrySetTextureDiffuse(const std::string &filename);

		float GetMetallic() const { return m_metallic; }

		void SetMetallic(const float &metallic) { m_metallic = metallic; }

		float GetRoughness() const { return m_roughness; }

		void SetRoughness(const float &roughness) { m_roughness = roughness; }

		bool GetCastsShadows() const { return m_castsShadows; }

		void SetCastsShadows(const bool &castsShadows) { m_castsShadows = castsShadows; }

		bool GetIgnoreLighting() const { return m_ignoreLighting; }

		void SetIgnoreLighting(const bool &ignoreLighting) { m_ignoreLighting = ignoreLighting; }

		bool GetIgnoreFog() const { return m_ignoreFog; }

		void SetIgnoreFog(const bool &ignoreFog) { m_ignoreFog = ignoreFog; }

		Texture *GetTextureMaterial() const { return m_textureMaterial; }

		void SetTextureMaterial(Texture *material) { m_textureMaterial = material; }

		void TrySetTextureMaterial(const std::string &filename);

		Texture *GetTextureNormal() const { return m_textureNormal; }

		void SetTextureNormal(Texture *normal) { m_textureNormal = normal; }

		void TrySetTextureNormal(const std::string &filename);
	};
}
