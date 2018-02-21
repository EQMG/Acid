#pragma once

#include "../../Objects/Component.hpp"
#include "../../Textures/Texture.hpp"
#include "../EntityRender.hpp"

namespace Flounder
{
	class F_EXPORT ComponentSway :
		public Component
	{
	private:
		Texture *m_textureSwayMap;
	public:
		ComponentSway(Texture *swayMap);

		ComponentSway(ComponentPrefab *prefab);

		~ComponentSway();

		void Update() override;

		void CmdRender(EntityRender *entityRender);

		std::string GetName() const override { return "ComponentSway"; };

		Texture *GetTextureSwayMap() const { return m_textureSwayMap; }

		void SetTextureSwayMap(Texture *swayMap) { m_textureSwayMap = swayMap; }
	};
}
