#pragma once

#include <Engine/Engine.hpp>
#include <Maths/Noise/Noise.hpp>
#include <Maths/Visual/DriverLinear.hpp>
#include <Maths/Vector3.hpp>
#include <Maths/Colour.hpp>
#include <Lights/Fog.hpp>

using namespace fl;

namespace test
{
	/// <summary>
	/// A module used for managing the world.
	/// </summary>
	class World :
		public IModule
	{
	private:
		DriverLinear m_driverDay;
		float m_factorDay;

		Fog m_fog;

		Vector3 m_skyboxRotation;
		Vector3 m_lightDirection;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static World *Get()
		{
			return Engine::Get()->GetModule<World>();
		}

		/// <summary>
		/// Creates a new worlds module.
		/// </summary>
		World();

		/// <summary>
		/// Deconstructor for the worlds module.
		/// </summary>
		~World();

		void Update() override;

		float GetDayFactor() const;

		float GetSunriseFactor() const;

		float GetShadowFactor() const;

		float GetStarIntensity() const;

		Fog GetFog() const { return m_fog; }

		Vector3 GetSkyboxRotation() const { return m_skyboxRotation; }

		Vector3 GetLightDirection() const { return m_lightDirection; }
	};
}