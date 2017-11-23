#pragma once

#include "../IComponent.hpp"
#include "../../lights/Light.hpp"

namespace Flounder
{
	class ComponentLight :
		public IComponent
	{
	private:
		Light *m_light;
	public:
		ComponentLight(const Light &light);

		~ComponentLight();

		void Update() override;

		void CmdRender(EntityRender *entityRender) override;

		Light *GetLight() const { return m_light; }

		void SetLight(const Light &light) const { m_light->Set(light); }
	};
}
