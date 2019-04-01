#pragma once

#include "Maths/Vector3.hpp"
#include "Maths/Transform.hpp"
#include "Scenes/Component.hpp"

namespace acid
{
/**
 * Component interface that defines a emitter volume.
 */
class ACID_EXPORT Emitter :
	public Component
{
public:
	/**
	 * Creates a new emitter.
	 * @param localTransform The local transform from the parents space.
	 */
	explicit Emitter(const Transform &localTransform = Transform::Identity) :
		m_localTransform(localTransform)
	{
	}

	/**
	 * Creates a new objects position.
	 * @return The new objects position.
	 */
	virtual Vector3 GeneratePosition() const = 0;

	const Transform &GetLocalTransform() const { return m_localTransform; }

	void SetLocalTransform(const Transform &localTransform) { m_localTransform = localTransform; }

protected:
	Transform m_localTransform;
};
}
