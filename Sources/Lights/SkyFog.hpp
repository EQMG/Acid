#pragma once

#include "Engine/Exports.hpp"
#include "Maths/Colour.hpp"

namespace acid
{
	/// <summary>
	/// Represents a hazy fog in the world.
	/// </summary>
	class ACID_EXPORT SkyFog
	{
	private:
		Colour m_colour;
		float m_density;
		float m_gradient;
		float m_lowerLimit;
		float m_upperLimit;
	public:
		/// <summary>
		/// Creates a new SkyFog.
		/// </summary>
		/// <param name="colour"> The colour of the SkyFog. </param>
		/// <param name="density"> How dense the SkyFog will be. </param>
		/// <param name="gradient"> The gradient of the SkyFog. </param>
		/// <param name="lowerLimit"> At what height will the skybox SkyFog begin to appear. </param>
		/// <param name="upperLimit"> At what height will there be skybox no SkyFog. </param>
		SkyFog(const Colour &colour, const float &density, const float &gradient, const float &lowerLimit, const float &upperLimit);

		/// <summary>
		/// Deconstructor for SkyFog.
		/// </summary>
		~SkyFog();

		Colour GetColour() const { return m_colour; }

		void SetColour(const Colour &colour) { m_colour = colour; }

		float GetDensity() const { return m_density; }

		void SetDensity(const float &density) { m_density = density; }

		float GetGradient() const { return m_gradient; }

		void SetGradient(const float &gradient) { m_gradient = gradient; }

		float GetLowerLimit() const { return m_lowerLimit; }

		void SetLowerLimit(const float &lowerLimit) { m_lowerLimit = lowerLimit; }

		float GetUpperLimit() const { return m_upperLimit; }

		void SetUpperLimit(const float &upperLimit) { m_upperLimit = upperLimit; }

		/// <summary>
		/// Saves this vector into a loaded value.
		/// </summary>
		/// <param name="destination"> The destination loaded value. </param>
		void Write(LoadedValue *destination);

		SkyFog &operator=(const SkyFog &other);

		SkyFog &operator=(LoadedValue *value);
	};
}
