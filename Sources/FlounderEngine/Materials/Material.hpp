#pragma once

#include "../Objects/Component.hpp"
#include "../Maths/Colour.hpp"
#include "../Textures/Texture.hpp"

namespace Flounder
{
	class F_EXPORT MaterialDiffuse
	{
	private:
		Colour *m_baseColor;
		Texture *m_texture;
	public:
		MaterialDiffuse(const Colour &baseColor = Colour::WHITE, Texture *texture = nullptr) :
			m_baseColor(new Colour(baseColor)),
			m_texture(texture)
		{
		}

		void Load(LoadedValue *value)
		{
			if (value == nullptr)
			{
				return;
			}
			TrySetTexture(value->GetChild("Texture")->GetString());
			m_baseColor->Set(value->GetChild("Colour")->GetString());
		}

		void Write(LoadedValue *destination)
		{
			destination->GetChild("Texture", true)->SetString(m_texture == nullptr ? "" : m_texture->GetFilename());
			destination->GetChild("Colour", true)->SetString(Colour::GetHex(*m_baseColor));
		}

		Colour *GetBaseColor() const { return m_baseColor; }

		void SetBaseColor(const Colour &baseColor) { m_baseColor->Set(baseColor); }

		Texture *GetTexture() const { return m_texture; }

		void SetTexture(Texture *texture) { m_texture = texture; }

		void TrySetTexture(const std::string &filename)
		{
			if (!filename.empty())
			{
				m_texture = Texture::Resource(filename);
			}
		}
	};

	class F_EXPORT MaterialSurface
	{
	private:
		float m_metallic;
		float m_roughness;
		bool m_ignoreLighting;
		bool m_ignoreFog;
		bool m_ignoreCurvature;
		Texture *m_texture; // R: Metallic, G: Roughness, B: Emmisive
	public:
		MaterialSurface(const float &metallic = 0.0f, const float &roughness = 0.0f, const bool &ignoreLighting = false, const bool &ignoreFog = false, const bool &ignoreCurvature = false, Texture *texture = nullptr) :
			m_metallic(metallic),
			m_roughness(roughness),
			m_ignoreLighting(ignoreLighting),
			m_ignoreFog(ignoreFog),
			m_ignoreCurvature(ignoreCurvature),
			m_texture(texture)
		{
		}

		void Load(LoadedValue *value)
		{
			if (value == nullptr)
			{
				return;
			}
			m_metallic = value->GetChild("Metallic")->Get<float>();
			m_roughness = value->GetChild("Roughness")->Get<float>();
			m_ignoreLighting = value->GetChild("Ignore Lighting")->Get<bool>();
			m_ignoreFog = value->GetChild("Ignore Fog")->Get<bool>();
			m_ignoreCurvature = value->GetChild("Ignore Curvature")->Get<bool>();
			TrySetTexture(value->GetChild("Texture")->GetString());
		}

		void Write(LoadedValue *destination)
		{
			destination->GetChild("Metallic", true)->Set(m_metallic);
			destination->GetChild("Roughness", true)->Set(m_roughness);
			destination->GetChild("Ignore Lighting", true)->Set(m_ignoreLighting);
			destination->GetChild("Ignore Fog", true)->Set(m_ignoreFog);
			destination->GetChild("Ignore Curvature", true)->Set(m_ignoreCurvature);
			destination->GetChild("Texture", true)->SetString(m_texture == nullptr ? "" : m_texture->GetFilename());
		}

		float GetMetallic() const { return m_metallic; }

		void SetMetallic(const float &metallic) { m_metallic = metallic; }

		float GetRoughness() const { return m_roughness; }

		void SetRoughness(const float &roughness) { m_roughness = roughness; }

		bool GetIgnoreLighting() const { return m_ignoreLighting; }

		void SetIgnoreLighting(const bool &ignoreLighting) { m_ignoreLighting = ignoreLighting; }

		bool GetIgnoreFog() const { return m_ignoreFog; }

		void SetIgnoreFog(const bool &ignoreFog) { m_ignoreFog = ignoreFog; }

		bool GetIgnoreCurvature() const { return m_ignoreCurvature; }

		void SetIgnoreCurvature(const bool &ignoreCurvature) { m_ignoreCurvature = ignoreCurvature; }

		Texture *GetTexture() const { return m_texture; }

		void SetTexture(Texture *texture) { m_texture = texture; }

		void TrySetTexture(const std::string &filename)
		{
			if (!filename.empty())
			{
				m_texture = Texture::Resource(filename);
			}
		}
	};

	class F_EXPORT MaterialNormal
	{
	private:
		Texture *m_texture;
	public:
		MaterialNormal(Texture *texture = nullptr) :
			m_texture(texture)
		{
		}

		void Load(LoadedValue *value)
		{
			if (value == nullptr)
			{
				return;
			}
			TrySetTexture(value->GetChild("Texture")->GetString());
		}

		void Write(LoadedValue *destination)
		{
			destination->GetChild("Texture", true)->SetString(m_texture == nullptr ? "" : m_texture->GetFilename());
		}

		Texture *GetTexture() const { return m_texture; }

		void SetTexture(Texture *texture) { m_texture = texture; }

		void TrySetTexture(const std::string &filename)
		{
			if (!filename.empty())
			{
				m_texture = Texture::Resource(filename);
			}
		}
	};

	class F_EXPORT MaterialSway
	{
	private:
		Texture *m_texture;
	public:
		MaterialSway(Texture *texture = nullptr) :
			m_texture(texture)
		{
		}

		void Load(LoadedValue *value)
		{
			if (value == nullptr)
			{
				return;
			}
			TrySetTexture(value->GetChild("Texture", true)->GetString());
		}

		void Write(LoadedValue *destination)
		{
			destination->GetChild("Texture", true)->SetString(m_texture == nullptr ? "" : m_texture->GetFilename());
		}

		Texture *GetTexture() const { return m_texture; }

		void SetTexture(Texture *texture) { m_texture = texture; }

		void TrySetTexture(const std::string &filename)
		{
			if (!filename.empty())
			{
				m_texture = Texture::Resource(filename);
			}
		}
	};

	/// <summary>
	/// Class that represents a material.
	/// </summary>
	class F_EXPORT Material :
		public Component
	{
	private:
		MaterialDiffuse *m_diffuse;
		MaterialSurface *m_surface;
		MaterialNormal *m_normal;
		MaterialSway *m_sway;

	public:
		Material(MaterialDiffuse *diffuse = new MaterialDiffuse(), MaterialSurface *surface = new MaterialSurface(), MaterialNormal *normal = new MaterialNormal(), MaterialSway *sway = new MaterialSway());

		~Material();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		std::string GetName() const override { return "Material"; };

		MaterialDiffuse *GetDiffuse() const { return m_diffuse; }

		MaterialSurface *GetSurface() const { return m_surface; }

		MaterialNormal *GetNormal() const { return m_normal; }

		MaterialSway *GetSway() const { return m_sway; }
	};
}
