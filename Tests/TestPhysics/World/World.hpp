#pragma once

#include <Engine/Engine.hpp>
#include <Maths/Visual/DriverLinear.hpp>
#include <Maths/Vector3.hpp>
#include <Maths/Colour.hpp>
#include <Lights/Fog.hpp>

using namespace acid;

namespace test
{
	/// <summary>
	/// A module used for managing the world.
	/// </summary>
	class World :
		public Module
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
		static World *Get() { return Engine::Get()->GetModuleManager().Get<World>(); }

		World();

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