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
	public:
		ComponentModel(Model *model);

		ComponentModel(ComponentPrefab *prefab);

		~ComponentModel();

		void Update() override;

		void CmdRender(EntityRender *entityRender) override;

		void Save(ComponentPrefab *prefab) override;

		std::string GetName() const override { return "ComponentModel"; };

		Model *GetModel() const { return m_model; }

		void SetModel(Model *model) { m_model = model; }
	};
}
