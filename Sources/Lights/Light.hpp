#pragma once

#include "Maths/Colour.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Transform.hpp"
#include "Scenes/Component.hpp"

namespace acid
{
/**
 * @brief Class that represents a point light.
 */
class ACID_EXPORT Light :
	public Component
{
public:
	/**
	 * Creates a new point light.
	 * @param colour The colour of the light.
	 * @param radius How far the light will have influence (-1 sets this to a directional light).
	 * @param localTransform The local transform from the parents space.
	 */
	explicit Light(const Colour &colour = Colour::White, const float &radius = -1.0f, const Transform &localTransform = Transform::Zero);

	void Start() override;

	void Update() override;

	void Decode(const Metadata &metadata) override;

	void Encode(Metadata &metadata) const override;

	const Colour &GetColour() const { return m_colour; }

	void SetColour(const Colour &colour) { m_colour = colour; }

	const float &GetRadius() const { return m_radius; }

	void SetRadius(const float &radius) { m_radius = radius; }

	const Transform &GetLocalTransform() const { return m_localTransform; }

	void SetLocalTransform(const Transform &localTransform) { m_localTransform = localTransform; }

	Transform GetWorldTransform() const;

private:
	Colour m_colour;
	Vector3f m_position;
	float m_radius;
	Transform m_localTransform;
	mutable Transform m_worldTransform;
};
}
