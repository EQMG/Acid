#pragma once

#include "../IComponent.hpp"
#include "../../Textures/Texture.hpp"

namespace Flounder
{
	class ComponentGlow :
		public IComponent
	{
	private:
		Texture *m_textureGlowMap;
	public:
		ComponentGlow(Texture *swayMap);

		ComponentGlow(ComponentPrefab* prefab);

		~ComponentGlow();

		void Update() override;

		void CmdRender(EntityRender *entityRender) override;

		void Save(ComponentPrefab* prefab) override;

		Texture *GetTextureGlowMap() const { return m_textureGlowMap; }

		void SetTextureGlowMap(Texture *glowMap) { m_textureGlowMap = glowMap; }
	};
}
