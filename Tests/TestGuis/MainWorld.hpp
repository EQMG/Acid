#pragma once

#include <Worlds/IWorld.hpp>

using namespace fl;

namespace test
{
	class MainWorld :
		public IWorld
	{
	private:
		Noise m_noiseTerrain;

		DriverLinear m_driverDay;
		float m_factorDay;

		Vector3 m_skyboxRotation;
		Vector3 m_sunPosition;
		Vector3 m_moonPosition;
		Colour m_sunColour;
		Colour m_moonColour;

		Fog m_fog;
		Colour m_skyColour;
	public:
		MainWorld();

		~MainWorld();

		void Update() override;

		float GetDayFactor() const override;

		float GetSunriseFactor() const override;

		float GetShadowFactor() const override;

		float GetSunHeight() const override;

		float GetStarIntensity() const override;

		float GetTerrainRadius(const float &radius, const float &theta, const float &phi) override;

		Noise GetNoiseTerrain() const override { return m_noiseTerrain; }

		Vector3 GetSkyboxRotation() const override { return m_skyboxRotation; }

		Vector3 GetSunPosition() const override { return m_sunPosition; }

		Vector3 GetMoonPosition() const override { return m_moonPosition; }

		Colour GetSunColour() const override { return m_sunColour; }

		Colour GetMoonColour() const override { return m_moonColour; }

		Fog GetFog() const override { return m_fog; }

		void SetFog(const Fog &fog) override { m_fog = fog; }

		Colour GetSkyColour() const override { return m_skyColour; }

		void SetSkyColour(const Colour &skyColour) override { m_skyColour = skyColour; }
	};
}