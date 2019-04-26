#pragma once

#include "Maths/Vector3.hpp"
#include "Maths/Transform.hpp"
#include "Scenes/Component.hpp"

namespace acid
{
/**
 * @brief Component interface that defines a emitter volume.
 */
class ACID_EXPORT Emitter :
	public Component
{
public:
	/**
	 * Creates a new emitter.
	 * @param localTransform The local transform from the parents space.
	 */
	explicit Emitter(const Transform &localTransform = Transform::Zero) :
		m_localTransform(localTransform)
	{
	}

	/**
	 * Creates a new objects position.
	 * @return The new objects position.
	 */
	virtual Vector3f GeneratePosition() const = 0;

	static Vector3f RandomUnitVector()
	{
		auto theta = Maths::Random(0.0f, 1.0f) * 2.0f * Maths::Pi;
		auto z = Maths::Random(0.0f, 1.0f) * 2.0f - 1.0f;
		auto rootOneMinusZSquared = std::sqrt(1.0f - z * z);
		auto x = rootOneMinusZSquared * std::cos(theta);
		auto y = rootOneMinusZSquared * std::sin(theta);
		return Vector3f(x, y, z);
	}

	const Transform &GetLocalTransform() const { return m_localTransform; }

	void SetLocalTransform(const Transform &localTransform) { m_localTransform = localTransform; }

	friend const Metadata& operator>>(const Metadata& metadata, Emitter& emitter)
	{
		// TODO: Virtual?
		return metadata;
	}

	friend Metadata& operator<<(Metadata& metadata, const Emitter& emitter)
	{
		// TODO: Virtual?
		return metadata;
	}

protected:
	Transform m_localTransform;
};
}
