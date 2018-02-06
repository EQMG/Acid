#pragma once

#include "../IComponent.hpp"
#include "../../Models/Model.hpp"
#include "../../Textures/Texture.hpp"

namespace Flounder
{
	class ComponentModel :
		public IComponent
	{
	private:
		Model *m_model;
		Texture *m_textureDiffuse;
	public:
		ComponentModel(Model *model, Texture *diffuse);

		ComponentModel(ComponentPrefab* prefab);

		~ComponentModel();

		void Update() override;

		void CmdRender(EntityRender *entityRender) override;

		Model *GetModel() const { return m_model; }

		void SetModel(Model *model) { m_model = model; }

		Texture *GetTextureDiffuse() const { return m_textureDiffuse; }

		void SetTextureDiffuse(Texture *diffuse) { m_textureDiffuse = diffuse; }
	};
}
