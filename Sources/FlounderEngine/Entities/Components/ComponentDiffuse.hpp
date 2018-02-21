#pragma once

#include "../../Objects/Component.hpp"
#include "../../Textures/Texture.hpp"
#include "../EntityRender.hpp"

namespace Flounder
{
	class F_EXPORT ComponentDiffuse :
		public Component
	{
	private:
		Texture *m_textureDiffuse;
	public:
		ComponentDiffuse(Texture *diffuse);

		ComponentDiffuse(ComponentPrefab *prefab);

		~ComponentDiffuse();

		void Update() override;

		void CmdRender(EntityRender *entityRender);

		std::string GetName() const override { return "ComponentDiffuse"; };

		Texture *GetTextureDiffuse() const { return m_textureDiffuse; }

		void SetTextureDiffuse(Texture *diffuse) { m_textureDiffuse = diffuse; }
	};
}
