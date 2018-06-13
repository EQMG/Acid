#pragma once

#include "Engine/Engine.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Visual/DriverLinear.hpp"
#include "Maths/Noise/Noise.hpp"
#include "Lights/Fog.hpp"

namespace fl
{
	/// <summary>
	/// A object used to represent a world.
	/// </summary>
	class FL_EXPORT IWorld
	{
	private:
	public:
		/// <summary>
		/// Creates a new world.
		/// </summary>
		IWorld()
		{
		}

		/// <summary>
		/// Deconstructor for the scene.
		/// </summary>
		virtual ~IWorld()
		{
		}

		/// <summary>
		/// Run when updating the world.
		/// </summary>
		virtual void Update() = 0;

		virtual float GetDayFactor() const = 0;

		virtual float GetSunriseFactor() const = 0;

		virtual float GetShadowFactor() const = 0;

		virtual float GetSunHeight() const = 0;

		virtual float GetStarIntensity() const = 0;

		virtual float GetTerrainRadius(const float &radius, const float &theta, const float &phi) = 0;

		virtual Noise GetNoiseTerrain() const = 0;

		virtual Vector3 GetSkyboxRotation() const = 0;

		virtual Vector3 GetSunPosition() const = 0;

		virtual Vector3 GetMoonPosition() const = 0;

		virtual Colour GetSunColour() const = 0;

		virtual Colour GetMoonColour() const = 0;

		virtual Fog GetFog() const = 0;

		virtual void SetFog(const Fog &fog) = 0;

		virtual Colour GetSkyColour() const = 0;

		virtual void SetSkyColour(const Colour &skyColour) = 0;
	};
}
