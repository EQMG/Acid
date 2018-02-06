#pragma once

#include "../IComponent.hpp"
#include "../../Textures/Texture.hpp"

namespace Flounder
{
	class ComponentSway :
		public IComponent
	{
	private:
		Texture *m_textureSwayMap;
	public:
		ComponentSway(Texture *swayMap);

		ComponentSway(ComponentPrefab* prefab);

		~ComponentSway();

		void Update() override;

		void CmdRender(EntityRender *entityRender) override;

		Texture *GetTextureSwayMap() const { return m_textureSwayMap; }

		void SetTextureSwayMap(Texture *swayMap) { m_textureSwayMap = swayMap; }
	};
}
