#pragma once

#include "../../Objects/Component.hpp"
#include "../EntityRender.hpp"

namespace Flounder
{
	enum CelestialType
	{
		CelestialSun = 0,
		CelestialMoon = 1
	};

	class F_EXPORT ComponentCelestial :
		public Component
	{
	private:
		CelestialType m_type;
	public:
		ComponentCelestial(const CelestialType &type);

		ComponentCelestial(ComponentPrefab *prefab);

		~ComponentCelestial();

		void Update() override;

		void CmdRender(EntityRender *entityRender);

		std::string GetName() const override { return "ComponentCelestial"; };
	};
}
