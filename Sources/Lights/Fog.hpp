#pragma once

#include "Maths/Colour.hpp"
#include "Serialized/Node.hpp"
#include "Scenes/Component.hpp"

namespace acid {
/**
 * @brief Component that represents a 3d fog.
 */
class ACID_EXPORT
Fog
:
public
Component
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
	explicit Fog(const Colour &colour = Colour::White, float density = 0.0f, float gradient = -1.0f, float lowerLimit = 0.0f, float upperLimit = 0.0f);

	void Start() override;

	void Update() override;

	const Colour &GetColour() const { return m_colour; }

	void SetColour(const Colour &colour) { m_colour = colour; }

	float GetDensity() const { return m_density; }

	void SetDensity(float density) { m_density = density; }

	float GetGradient() const { return m_gradient; }

	void SetGradient(float gradient) { m_gradient = gradient; }

	float GetLowerLimit() const { return m_lowerLimit; }

	void SetLowerLimit(float lowerLimit) { m_lowerLimit = lowerLimit; }

	float GetUpperLimit() const { return m_upperLimit; }

	void SetUpperLimit(float upperLimit) { m_upperLimit = upperLimit; }

	friend const Node &operator>>(const Node &node, Fog &fog);

	friend Node &operator<<(Node &node, const Fog &fog);

private:
	Colour m_colour;
	float m_density;
	float m_gradient;
	float m_lowerLimit;
	float m_upperLimit;
};
}
