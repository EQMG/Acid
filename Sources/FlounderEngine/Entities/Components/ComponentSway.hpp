#pragma once

#include "../IComponent.hpp"
#include "../../Textures/Texture.hpp"

namespace Flounder
{
	class F_EXPORT ComponentSway :
		public IComponent
	{
	private:
		Texture *m_textureSwayMap;
	public:
		ComponentSway(Texture *swayMap);

		ComponentSway(ComponentPrefab *prefab);

		~ComponentSway();

		void Update() override;

		void CmdRender(EntityRender *entityRender) override;

		void Save(ComponentPrefab *prefab) override;

		std::string GetName() const override { return "ComponentSway"; };

		Texture *GetTextureSwayMap() const { return m_textureSwayMap; }

		void SetTextureSwayMap(Texture *swayMap) { m_textureSwayMap = swayMap; }
	};
}
