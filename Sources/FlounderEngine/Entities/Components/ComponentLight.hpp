#pragma once

#include "../../Objects/Component.hpp"
#include "../../Lights/Light.hpp"
#include "../EntityRender.hpp"

namespace Flounder
{
	class F_EXPORT ComponentLight :
		public Component
	{
	private:
		Light *m_light;
		Vector3 *m_offset;
	public:
		ComponentLight(const Light &light, const Vector3 &offset);

		ComponentLight(ComponentPrefab *prefab);

		~ComponentLight();

		void Update() override;

		void CmdRender(EntityRender *entityRender);

		std::string GetName() const override { return "ComponentLight"; };

		Light *GetLight() const { return m_light; }

		void SetLight(const Light &light) const { m_light->Set(light); }

		Vector3 *GetOffset() const { return m_offset; }

		void SetOffset(const Vector3 &offset) const { m_offset->Set(offset); }
	};
}
