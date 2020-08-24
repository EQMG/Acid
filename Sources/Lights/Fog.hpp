#pragma once

#include "Maths/Colour.hpp"
#include "Files/Node.hpp"
#include "Scenes/Component.hpp"

namespace acid {
/**
 * @brief Component that represents a 3d fog.
 */
class ACID_EXPORT Fog : public Component::Registrar<Fog> {
	static const bool Registered;
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

	const Colour &GetColour() const { return colour; }
	void SetColour(const Colour &colour) { this->colour = colour; }

	float GetDensity() const { return density; }
	void SetDensity(float density) { this->density = density; }

	float GetGradient() const { return gradient; }
	void SetGradient(float gradient) { this->gradient = gradient; }

	float GetLowerLimit() const { return lowerLimit; }
	void SetLowerLimit(float lowerLimit) { this->lowerLimit = lowerLimit; }

	float GetUpperLimit() const { return upperLimit; }
	void SetUpperLimit(float upperLimit) { this->upperLimit = upperLimit; }

	friend const Node &operator>>(const Node &node, Fog &fog);
	friend Node &operator<<(Node &node, const Fog &fog);

private:
	Colour colour;
	float density;
	float gradient;
	float lowerLimit, upperLimit;
};
}
