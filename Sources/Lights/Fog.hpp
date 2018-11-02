#pragma once

#include "Maths/Colour.hpp"

namespace acid
{
	class Packet;

	class Metadata;

	/// <summary>
	/// Represents a hazy fog in the world.
	/// </summary>
	class ACID_EXPORT Fog
	{
	private:
		Colour m_colour;
		float m_density;
		float m_gradient;
		float m_lowerLimit;
		float m_upperLimit;
	public:
		/// <summary>
		/// Constructor for Fog.
		/// </summary>
		/// <param name="colour"> The colour of the Fog. </param>
		/// <param name="density"> How dense the Fog will be. </param>
		/// <param name="gradient"> The gradient of the Fog. </param>
		/// <param name="lowerLimit"> At what height will the skybox Fog begin to appear. </param>
		/// <param name="upperLimit"> At what height will there be skybox no Fog. </param>
		explicit Fog(const Colour &colour = Colour::WHITE, const float &density = 0.0f, const float &gradient = -1.0f, const float &lowerLimit = 0.0f, const float &upperLimit = 0.0f);

		/// <summary>
		/// Constructor for Fog.
		/// </summary>
		/// <param name="source"> Creates this fog out of a existing one. </param>
		Fog(const Fog &source);

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

		void Decode(const Metadata &metadata);

		void Encode(Metadata &metadata) const;

		ACID_EXPORT friend std::ostream &operator<<(std::ostream &stream, const Fog &fog);

		ACID_EXPORT friend Packet &operator<<(Packet &packet, const Fog &fog);

		ACID_EXPORT friend Packet &operator>>(Packet &packet, Fog &fog);

		std::string ToString() const;
	};
}
