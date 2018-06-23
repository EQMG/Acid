#pragma once

#include <Objects/IComponent.hpp>
#include <Objects/GameObject.hpp>

using namespace fl;

namespace test
{
	enum CelestialType
	{
		CELESTIAL_SUN = 0,
		CELESTIAL_MOON = 1
	};

	class CelestialBody :
		public IComponent
	{
	private:
		CelestialType m_type;
	public:
		CelestialBody(const CelestialType &type = CELESTIAL_SUN);

		~CelestialBody();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		std::string GetName() const override { return "CelestialBody"; };

		CelestialType GetType() const { return m_type; }

		void SetType(const CelestialType &type) { m_type = type; }
	};
}
