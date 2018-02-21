#pragma once

#include "../../Objects/Component.hpp"
#include "../../Textures/Texture.hpp"
#include "../EntityRender.hpp"

namespace Flounder
{
	class F_EXPORT ComponentNormal :
		public Component
	{
	private:
		Texture *m_textureNormal;
	public:
		ComponentNormal(Texture *normal);

		ComponentNormal(ComponentPrefab *prefab);

		~ComponentNormal();

		void Update() override;

		void CmdRender(EntityRender *entityRender);

		std::string GetName() const override { return "ComponentNormal"; };

		Texture *GetTextureNormal() const { return m_textureNormal; }

		void SetTextureNormal(Texture *normal) { m_textureNormal = normal; }
	};
}
