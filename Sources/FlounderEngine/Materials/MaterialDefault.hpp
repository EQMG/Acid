#pragma once

#include "../Objects/Component.hpp"
#include "../Maths/Colour.hpp"
#include "../Textures/Texture.hpp"

namespace Flounder
{
	/// <summary>
	/// Class that represents the default material shader.
	/// </summary>
	class F_EXPORT MaterialDefault :
		public Component
	{
	private:
		Colour *m_baseColor;
		Texture *m_diffuseTexture;

		float m_metallic;
		float m_roughness;
		bool m_ignoreLighting;
		bool m_ignoreFog;
		Texture *m_materialTexture;

		Texture *m_normalTexture;
	public:
		MaterialDefault(const Colour &baseColor = Colour::WHITE, Texture *diffuseTexture = nullptr,
				 const float &metallic = 0.0f, const float &roughness = 0.0f, const bool &ignoreLighting = false, const bool &ignoreFog = false, Texture *materialTexture = nullptr,
				 Texture *normalTexture = nullptr);

		~MaterialDefault();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		std::string GetName() const override { return "MaterialDefault"; };

		Colour *GetBaseColor() const { return m_baseColor; }

		void SetBaseColor(const Colour &baseColor) { m_baseColor->Set(baseColor); }

		Texture *GetDiffuseTexture() const { return m_diffuseTexture; }

		void SetDiffuseTexture(Texture *diffuseTexture) { m_diffuseTexture = diffuseTexture; }

		void TrySetDiffuseTexture(const std::string &filename)
		{
			if (!filename.empty())
			{
				m_diffuseTexture = Texture::Resource(filename);
			}
		}

		float GetMetallic() const { return m_metallic; }

		void SetMetallic(const float &metallic) { m_metallic = metallic; }

		float GetRoughness() const { return m_roughness; }

		void SetRoughness(const float &roughness) { m_roughness = roughness; }

		bool IsIgnoringLighting() const { return m_ignoreLighting; }

		void SetIgnoreLighting(const bool &ignoreLighting) { m_ignoreLighting = ignoreLighting; }

		bool IsIgnoringFog() const { return m_ignoreFog; }

		void SetIgnoreFog(const bool &ignoreFog) { m_ignoreFog = ignoreFog; }

		Texture *GetMaterialTexture() const { return m_materialTexture; }

		void SetMaterialTexture(Texture *materialTexture) { m_materialTexture = materialTexture; }

		void TrySetMaterialTexture(const std::string &filename)
		{
			if (!filename.empty())
			{
				m_materialTexture = Texture::Resource(filename);
			}
		}

		Texture *GetNormalTexture() const { return m_normalTexture; }

		void SetNormalTexture(Texture *normalTexture) { m_normalTexture = normalTexture; }

		void TrySetNormalTexture(const std::string &filename)
		{
			if (!filename.empty())
			{
				m_normalTexture = Texture::Resource(filename);
			}
		}
	};
}
