#pragma once

#include "Objects/Component.hpp"
#include "Objects/GameObject.hpp"

namespace fl
{
	enum CelestialType
	{
		CELESTIAL_SUN = 0,
		CELESTIAL_MOON = 1
	};

	class FL_EXPORT CelestialBody :
		public Component
	{
	private:
		CelestialType m_type;
	public:
		CelestialBody(const CelestialType &type = CELESTIAL_SUN);

		~CelestialBody();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *value) override;

		std::string GetName() const override { return "CelestialBody"; };

		CelestialType GetType() const { return m_type; }

		void SetType(const CelestialType &type) { m_type = type; }
	};
}
