#pragma once

#include "../Maths/Vector3.hpp"
#include "../Engine/Engine.hpp"
#include "../Skyboxes/Skyboxes.hpp"
#include "../Visual/DriverLinear.hpp"

namespace Flounder
{
	/// <summary>
	/// A module used for managing worlds.
	/// </summary>
	class Worlds :
		public IModule
	{
	private:
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
			return dynamic_cast<Worlds *>(Engine::Get()->GetModule("worlds"));
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

		Vector3 *GetSunPosition() const { return m_sunPosition; }

		Colour *GetSunColour() const { return m_sunColour; }
	};
}
