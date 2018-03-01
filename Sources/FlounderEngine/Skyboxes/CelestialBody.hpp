#pragma once

#include "../Objects/Component.hpp"
#include "../Objects/GameObject.hpp"

namespace Flounder
{
	enum CelestialType
	{
		CelestialSun = 0,
		CelestialMoon = 1
	};

	class F_EXPORT CelestialBody :
		public Component
	{
	private:
		CelestialType m_type;
	public:
		CelestialBody(const CelestialType &type = CelestialType::CelestialSun);

		~CelestialBody();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *value) override;

		std::string GetName() const override { return "CelestialBody"; };

		CelestialType GetType() const { return m_type; }

		void SetType(const CelestialType &type) { m_type = type; }
	};
}
