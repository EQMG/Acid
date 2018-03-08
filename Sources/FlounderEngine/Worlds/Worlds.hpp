#pragma once

#include "../Engine/Engine.hpp"
#include "../Maths/Vector3.hpp"
#include "../Maths/Visual/DriverLinear.hpp"
#include "../Skyboxes/Skyboxes.hpp"

namespace Flounder
{
	/// <summary>
	/// A module used for managing worlds.
	/// </summary>
	class F_EXPORT Worlds :
		public IModule
	{
	private:
		DriverLinear *m_driverDay;
		float m_factorDay;

		Vector3 *m_skyboxRotation;
		Vector3 *m_sunPosition;
		Vector3 *m_moonPosition;
		Colour *m_sunColour;
		Colour *m_moonColour;
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

		Vector3 *GetSkyboxRotation() const { return m_skyboxRotation; }

		Vector3 *GetSunPosition() const { return m_sunPosition; }

		Vector3 *GetMoonPosition() const { return m_moonPosition; }

		Colour *GetSunColour() const { return m_sunColour; }

		Colour *GetMoonColour() const { return m_moonColour; }
	};
}
