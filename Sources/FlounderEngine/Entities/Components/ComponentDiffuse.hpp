#pragma once

#include "../IComponent.hpp"
#include "../../Textures/Texture.hpp"

namespace Flounder
{
	class ComponentDiffuse :
		public IComponent
	{
	private:
		Texture *m_textureDiffuse;
	public:
		ComponentDiffuse(Texture *diffuse);

		ComponentDiffuse(ComponentPrefab *prefab);

		~ComponentDiffuse();

		void Update() override;

		void CmdRender(EntityRender *entityRender) override;

		void Save(ComponentPrefab *prefab) override;

		std::string GetName() const override { return "ComponentDiffuse"; };

		Texture *GetTextureDiffuse() const { return m_textureDiffuse; }

		void SetTextureDiffuse(Texture *diffuse) { m_textureDiffuse = diffuse; }
	};
}
