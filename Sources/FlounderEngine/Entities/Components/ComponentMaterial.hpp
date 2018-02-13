#pragma once

#include "../../Maths/Colour.hpp"
#include "../../Textures/Texture.hpp"
#include "../IComponent.hpp"

namespace Flounder
{
	class F_EXPORT ComponentMaterial :
		public IComponent
	{
	private:
		float m_metallic;
		float m_roughness;
		Colour *m_diffuse;
		bool m_castsShadows;
		bool m_ignoreLighting;
		bool m_ignoreFog;

		Texture *m_textureMaterial;
	public:
		ComponentMaterial(const float &metallic = 0.0f, const float &roughness = 0.0f, const Colour &diffuse = Colour::WHITE, const bool &castsShadows = true, const bool &ignoreLighting = false, const bool &ignoreFog = false, Texture *material = nullptr);

		ComponentMaterial(ComponentPrefab *prefab);

		~ComponentMaterial();

		void Update() override;

		void CmdRender(EntityRender *entityRender) override;

		void Save(ComponentPrefab *prefab) override;

		std::string GetName() const override { return "ComponentMaterial"; };

		float GetMetallic() const { return m_metallic; }

		void SetMetallic(const float &metallic) { m_metallic = metallic; }

		float GetRoughness() const { return m_roughness; }

		void SetRoughness(const float &roughness) { m_roughness = roughness; }

		Colour *GetDiffuse() const { return m_diffuse; }

		void SetDiffuse(const Colour &diffuse) { m_diffuse->Set(diffuse); }

		bool GetCastsShadows() const { return m_castsShadows; }

		void SetCastsShadows(const bool &castsShadows) { m_castsShadows = castsShadows; }

		bool GetIgnoreLighting() const { return m_ignoreLighting; }

		void SetIgnoreLighting(const bool &ignoreLighting) { m_ignoreLighting = ignoreLighting; }

		bool GetIgnoreFog() const { return m_ignoreFog; }

		void SetIgnoreFog(const bool &ignoreFog) { m_ignoreFog = ignoreFog; }

		Texture *GetTextureMaterial() const { return m_textureMaterial; }

		void SetTextureMaterial(Texture *material) { m_textureMaterial = material; }
	};
}
