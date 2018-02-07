#pragma once

#include "../IComponent.hpp"

namespace Flounder
{
	class ComponentMaterial :
		public IComponent
	{
	private:
		float m_metallic;
		float m_roughness;
		bool m_castsShadows;
		bool m_ignoreLighting;
		bool m_ignoreFog;
	public:
		ComponentMaterial(const float &metallic = 0.0f, const float &roughness = 0.0f, const bool &castsShadows = true, const bool &ignoreLighting = false, const bool &ignoreFog = false);

		ComponentMaterial(ComponentPrefab* prefab);

		~ComponentMaterial();

		void Update() override;

		void CmdRender(EntityRender *entityRender) override;

		void Save(ComponentPrefab* prefab) override;

		std::string GetName() const override { return "ComponentMaterial"; };

		float GetMetallic() const { return m_metallic; }

		void SetMetallic(const float &metallic) { m_metallic = metallic; }

		float GetRoughness() const { return m_roughness; }

		void SetRoughness(const float &roughness) { m_roughness = roughness; }

		bool GetCastsShadows() const { return m_castsShadows; }

		void SetCastsShadows(const bool &castsShadows) { m_castsShadows = castsShadows; }
	};
}
