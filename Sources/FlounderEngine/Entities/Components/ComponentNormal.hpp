#pragma once

#include "../IComponent.hpp"
#include "../../Textures/Texture.hpp"

namespace Flounder
{
	class ComponentNormal :
		public IComponent
	{
	private:
		Texture *m_textureNormal;
	public:
		ComponentNormal(Texture *normal);

		ComponentNormal(ComponentPrefab *prefab);

		~ComponentNormal();

		void Update() override;

		void CmdRender(EntityRender *entityRender) override;

		void Save(ComponentPrefab *prefab) override;

		std::string GetName() const override { return "ComponentNormal"; };

		Texture *GetTextureNormal() const { return m_textureNormal; }

		void SetTextureNormal(Texture *normal) { m_textureNormal = normal; }
	};
}
