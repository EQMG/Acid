#pragma once

#include "Maths/Colour.hpp"
#include "Serialized/Metadata.hpp"

namespace acid
{
class Metadata;

/**
 * @brief Represents a hazy fog in the world.
 */
class ACID_EXPORT Fog
{
public:
	/**
	 * Creates a new hazy fog.
	 * @param colour The colour of the fog.
	 * @param density How dense the fog will be.
	 * @param gradient The gradient of the fog.
	 * @param lowerLimit At what height will the skybox fog begin to appear.
	 * @param upperLimit At what height will there be skybox no fog.
	 */
	explicit Fog(const Colour &colour = Colour::White, const float &density = 0.0f, const float &gradient = -1.0f, const float &lowerLimit = 0.0f, const float &upperLimit = 0.0f);

	const Colour &GetColour() const { return m_colour; }

	void SetColour(const Colour &colour) { m_colour = colour; }

	const float &GetDensity() const { return m_density; }

	void SetDensity(const float &density) { m_density = density; }

	const float &GetGradient() const { return m_gradient; }

	void SetGradient(const float &gradient) { m_gradient = gradient; }

	const float &GetLowerLimit() const { return m_lowerLimit; }

	void SetLowerLimit(const float &lowerLimit) { m_lowerLimit = lowerLimit; }

	const float &GetUpperLimit() const { return m_upperLimit; }

	void SetUpperLimit(const float &upperLimit) { m_upperLimit = upperLimit; }

	void Decode(const Metadata &metadata);

	void Encode(Metadata &metadata) const;

	ACID_EXPORT friend std::ostream &operator<<(std::ostream &stream, const Fog &fog);

	std::string ToString() const;

private:
	Colour m_colour;
	float m_density;
	float m_gradient;
	float m_lowerLimit;
	float m_upperLimit;
};
}
