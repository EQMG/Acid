#pragma once

#include "Maths/Colour.hpp"
#include "Maths/Vector3.hpp"
#include "Scenes/Component.hpp"
#include "ShimExport.hpp"

namespace acid {
/**
 * @brief Component that represents a point light.
 */
class ACID_SHIM_EXPORT Light : public Component::Registrar<Light> {
	inline static const bool Registered = Register("light");
public:
	/**
	 * Creates a new point light.
	 * @param colour The colour of the light.
	 * @param radius How far the light will have influence (-1 sets this to a directional light).
	 */
	explicit Light(const Colour &colour = Colour::White, float radius = -1.0f);

	void Start() override;
	void Update() override;

	const Colour &GetColour() const { return colour; }
	void SetColour(const Colour &colour) { this->colour = colour; }

	float GetRadius() const { return radius; }
	void SetRadius(float radius) { this->radius = radius; }

	friend const Node &operator>>(const Node &node, Light &light);
	friend Node &operator<<(Node &node, const Light &light);

private:
	Colour colour;
	Vector3f position;
	float radius;
};
}
