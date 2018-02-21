#pragma once

#include "../../Objects/Component.hpp"
#include "../../Models/Model.hpp"
#include "../../Textures/Texture.hpp"
#include "../EntityRender.hpp"

namespace Flounder
{
	class F_EXPORT ComponentModel :
		public Component
	{
	private:
		Model *m_model;
	public:
		ComponentModel(Model *model);

		ComponentModel(ComponentPrefab *prefab);

		~ComponentModel();

		void Update() override;

		void CmdRender(EntityRender *entityRender);

		std::string GetName() const override { return "ComponentModel"; };

		Model *GetModel() const { return m_model; }

		void SetModel(Model *model) { m_model = model; }
	};
}
