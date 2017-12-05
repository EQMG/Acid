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
	public:
		ComponentMaterial(const float &metallic, const float &roughness);

		~ComponentMaterial();

		void Update() override;

		void CmdRender(EntityRender *entityRender) override;

		float GetMetallic() const { return m_metallic; }

		void SetMetallic(const float &metallic) { m_metallic = metallic; }

		float GetRoughness() const { return m_roughness; }

		void SetRoughness(const float &roughness) { m_roughness = roughness; }
	};
}
