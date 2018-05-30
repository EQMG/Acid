#pragma once

#include "Engine/Engine.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Visual/DriverLinear.hpp"
#include "Maths/Noise/Noise.hpp"
#include "Lights/Fog.hpp"

namespace fl
{
	/// <summary>
	/// A module used for managing worlds.
	/// </summary>
	class FL_EXPORT Worlds :
		public IModule
	{
	private:
		static const Colour FOG_COLOUR_SUNRISE;
		static const Colour FOG_COLOUR_NIGHT;
		static const Colour FOG_COLOUR_DAY;

		static const Colour SUN_COLOUR_SUNRISE;
		static const Colour SUN_COLOUR_NIGHT;
		static const Colour SUN_COLOUR_DAY;

		static const Colour MOON_COLOUR_NIGHT;
		static const Colour MOON_COLOUR_DAY;

		static const Colour SKYBOX_COLOUR_DAY;

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
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Worlds *Get()
		{
			return reinterpret_cast<Worlds *>(Engine::Get()->GetModule("worlds"));
		}

		/// <summary>
		/// Creates a new worlds module.
		/// </summary>
		Worlds();

		/// <summary>
		/// Deconstructor for the worlds module.
		/// </summary>
		~Worlds();

		void Update() override;

		float GetDayFactor() const;

		float GetSunriseFactor() const;

		float GetShadowFactor() const;

		float GetSunHeight() const;

		float GetStarIntensity() const;

		float GetTerrainRadius(const float &radius, const float &theta, const float &phi);

		Noise GetNoiseTerrain() const { return m_noiseTerrain; }

		Vector3 GetSkyboxRotation() const { return m_skyboxRotation; }

		Vector3 GetSunPosition() const { return m_sunPosition; }

		Vector3 GetMoonPosition() const { return m_moonPosition; }

		Colour GetSunColour() const { return m_sunColour; }

		Colour GetMoonColour() const { return m_moonColour; }

		Fog GetFog() const { return m_fog; }

		void SetFog(const Fog &fog) { m_fog = fog; }

		Colour GetSkyColour() const { return m_skyColour; }

		void SetSkyColour(const Colour &skyColour) { m_skyColour = skyColour; }
	};
}
