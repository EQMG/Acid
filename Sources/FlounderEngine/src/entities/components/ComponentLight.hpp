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
		Vector3 *m_offset;
	public:
		ComponentLight(const Light &light, const Vector3 &offset);

		~ComponentLight();

		void Update() override;

		void CmdRender(EntityRender *entityRender) override;

		Light *GetLight() const { return m_light; }

		void SetLight(const Light &light) const { m_light->Set(light); }

		Vector3 *GetOffset() const { return m_offset; }

		void SetOffset(const Vector3 &offset) const { m_offset->Set(offset); }
	};
}
