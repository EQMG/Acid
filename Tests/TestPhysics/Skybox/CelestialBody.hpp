#pragma once

#include <Objects/Component.hpp>
#include <Objects/Entity.hpp>

using namespace acid;

namespace test
{
	enum CelestialType
	{
		CELESTIAL_SUN = 0,
		CELESTIAL_MOON = 1
	};

	class CelestialBody :
		public Component
	{
	private:
		CelestialType m_type;
	public:
		explicit CelestialBody(const CelestialType &type = CELESTIAL_SUN);

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;

		CelestialType GetType() const { return m_type; }

		void SetType(const CelestialType &type) { m_type = type; }
	};
}
