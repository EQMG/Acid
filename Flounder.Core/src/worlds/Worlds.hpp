#pragma once

#include "../maths/Vector3.hpp"
#include "../maths/Matrix4.hpp"
#include "../engine/Engine.hpp"
#include "../skyboxes/Skyboxes.hpp"
#include "../shadows/Shadows.hpp"
#include "../visual/DriverLinear.hpp"
#include "../noise/NoiseFast.hpp"

namespace Flounder
{
	/// <summary>
	/// A module used for managing worlds.
	/// </summary>
	class Worlds :
		public IModule
	{
	private:
		NoiseFast *m_noise;

		DriverLinear *m_driverDay;
		float m_factorDay;

		Vector3 *m_sunPosition;
		Colour *m_sunColour;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Worlds *Get()
		{
			return static_cast<Worlds*>(Engine::Get()->GetModule("worlds"));
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

		NoiseFast *GetNoise() const { return m_noise; }

		float GetDayFactor();

		float GetSunriseFactor();

		float GetShadowFactor();

		float GetSunHeight();

		float GetStarIntensity();

		Vector3 *GetSunPosition() const { return m_sunPosition; }

		Colour *GetSunColour() const { return m_sunColour; }
	};
}
