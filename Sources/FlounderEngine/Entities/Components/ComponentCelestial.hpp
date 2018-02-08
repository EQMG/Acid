#pragma once

#include "../IComponent.hpp"

namespace Flounder
{
	enum CelestialType
	{
		CelestialSun = 0,
		CelestialMoon = 1
	};

	class ComponentCelestial :
		public IComponent
	{
	private:
		CelestialType m_type;
	public:
		ComponentCelestial(const CelestialType &type);

		ComponentCelestial(ComponentPrefab *prefab);

		~ComponentCelestial();

		void Update() override;

		void CmdRender(EntityRender *entityRender) override;

		void Save(ComponentPrefab *prefab) override;

		std::string GetName() const override { return "ComponentCelestial"; };
	};
}
